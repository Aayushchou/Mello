#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================

std::unique_ptr<juce::RangedAudioParameter> createParam(const juce::String &paramID,
                                                        const juce::String &paramName,
                                                        float minValue,
                                                        float maxValue,
                                                        float defaultValue)
{
    return std::make_unique<juce::AudioParameterFloat>(paramID, paramName,
                                                       juce::NormalisableRange<float>(minValue, maxValue),
                                                       defaultValue);
}

std::unique_ptr<juce::RangedAudioParameter> createParamBool(const juce::String &paramID,
                                                            const juce::String &paramName,
                                                            float defaultValue)
{
    return std::make_unique<juce::AudioParameterBool>(paramID, paramName,
                                                      defaultValue);
}
//==============================================================================
MelloAudioProcessor::MelloAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("MelloParams"), createParameterLayout())
{
    _cutOffDepth = (getSampleRate() * 0.5);
    _phase = 0.0;

    envelopeParameters.attackParameter = parameters.getRawParameterValue(ParameterID::kEnvelopeAttack);
    envelopeParameters.releaseParameter = parameters.getRawParameterValue(ParameterID::kEnvelopeRelease);
    envelopeParameters.mappingParameter = parameters.getRawParameterValue(ParameterID::kEnvelopeMapping);

    vibratoParameters.minDepthParameter = parameters.getRawParameterValue(ParameterID::kVibratoMin);
    vibratoParameters.maxDepthParameter = parameters.getRawParameterValue(ParameterID::kVibratoMax);
    vibratoParameters.rateParameter = parameters.getRawParameterValue(ParameterID::kVibratoRate);

    filterParameters.lowOrBandParameter = parameters.getRawParameterValue(ParameterID::kLowOrBand);
    filterParameters.cutoffParameter = parameters.getRawParameterValue(ParameterID::kLowPassCutoff);
    filterParameters.resonanceParameter = parameters.getRawParameterValue(ParameterID::kLowPassResonance);
    filterParameters.driveParameter = parameters.getRawParameterValue(ParameterID::kLowPassDrive);

    mixerParameters.mixParameter = parameters.getRawParameterValue(ParameterID::kMix);
}

MelloAudioProcessor::~MelloAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MelloAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(createParam(ParameterID::kEnvelopeAttack, "Attack", 1.0, 50.0, 1.0));
    params.push_back(createParam(ParameterID::kEnvelopeRelease, "Release", 50.0, 500.0, 200));
    params.push_back(createParamBool(ParameterID::kEnvelopeMapping, "Mapping", false));
    params.push_back(createParamBool(ParameterID::kLowOrBand, "Low/Band", false));
    params.push_back(createParam(ParameterID::kLowPassCutoff, "Cutoff", 20.0, 10000.0, 400.0));
    params.push_back(createParam(ParameterID::kLowPassDrive, "Drive", 0.0, 20.0, 1.0));
    params.push_back(createParam(ParameterID::kLowPassResonance, "Resonance", 0.0, 0.9, 0.5));
    params.push_back(createParam(ParameterID::kVibratoMin, "Min", 0.0, 0.005, 0.0005));
    params.push_back(createParam(ParameterID::kVibratoMax, "Max", 0.005, 0.02, 0.005));
    params.push_back(createParam(ParameterID::kVibratoRate, "Rate", 0.0, 10.0, 3.0));
    params.push_back(createParam(ParameterID::kMix, "Mix", 0.0, 1.0, 0.7));
    return {params.begin(), params.end()};
}

//==============================================================================
const juce::String MelloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MelloAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MelloAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MelloAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MelloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MelloAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int MelloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MelloAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MelloAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MelloAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

int MelloAudioProcessor::getRandomInteger(int min, int max)
{
    // Create a random device and seed generator
    std::random_device rd;
    // Use a Mersenne Twister random number generator
    std::mt19937 rng(rd());
    // Define the range as 0 to max inclusive
    std::uniform_int_distribution<int> uni(min, max);

    // Generate and return a random integer within the defined range
    return uni(rng);
}

//==============================================================================
void MelloAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    _delayMax = *vibratoParameters.maxDepthParameter;
    int maxDelayInSamples = static_cast<int>(std::round(_delayMax * getSampleRate()));

    _invSampleRate = 1.0 / getSampleRate();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = (juce::uint32)getTotalNumInputChannels();

    // set up ladder filter
    _ladderFilter.prepare(spec);
    _ladderFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
    _ladderFilter.setEnabled(true);
    _ladderFilter.setResonance(0.7);
    _ladderFilter.setDrive(*filterParameters.driveParameter);

    // set up ballistic filter
    _ballisticsFilter.prepare(spec);
    _ballisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::peak);
    _ballisticsFilter.setAttackTime(50.0);
    _ballisticsFilter.setReleaseTime(500.0);

    // set up delay line
    _delayLine.prepare(spec);
    _delayLine.setMaximumDelayInSamples(maxDelayInSamples);
}

void MelloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool MelloAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void MelloAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                       juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    _delayMin = *vibratoParameters.minDepthParameter;
    _delayMax = *vibratoParameters.maxDepthParameter;
    _delayDiff = _delayMax - _delayMin;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // Define and clear buffers

    envelopeBuffer.setSize(totalNumInputChannels, numSamples);
    dryBuffer.setSize(totalNumInputChannels, numSamples);
    delayBuffer.setSize(1, numSamples);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        dryBuffer.copyFrom(channel, 0, buffer, channel, 0, numSamples);
        envelopeBuffer.copyFrom(channel, 0, buffer, channel, 0, numSamples);
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, numSamples);
        dryBuffer.clear(i, 0, numSamples);
        envelopeBuffer.clear(i, 0, numSamples);
        delayBuffer.clear(i, 0, numSamples);
    }

    // Determine envelop using ballistics filter

    juce::dsp::AudioBlock<float> controlBlock(envelopeBuffer);
    juce::dsp::ProcessContextReplacing<float> controlContext(controlBlock);
    _ballisticsFilter.process(controlContext);

    // Define delay buffer

    auto *delaySignal = delayBuffer.getWritePointer(0);

    auto *envelopeReader = envelopeBuffer.getReadPointer(0);
    float ph = _phase; // Make sure _phase is declared as a member variable and initialized to 0

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *wetSignal = buffer.getWritePointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float in = dryBuffer.getSample(channel, i); // Get the dry signal

            // Calculate LFO phase in radians
            float lfoPhase = ph * 2.0 * M_PI;

            float depth = _delayMin + envelopeReader[i] * _delayDiff;

            // Calculate current delay in seconds
            float currentDelay = depth * (0.5f + 0.5f * sinf(lfoPhase));
            // Convert delay time in seconds to sample length
            int delaySamples = static_cast<int>(std::round(currentDelay * getSampleRate()));

            // Push the dry sample into the delay line and get the delayed sample
            _delayLine.pushSample(channel, wetSignal[i]);
            wetSignal[i] = _delayLine.popSample(channel, delaySamples, true);

            // Update LFO phase
            ph += *vibratoParameters.rateParameter * _invSampleRate;
            if (ph >= 1.0)
            {
                ph -= 1.0;
            }
        }
    }

    _phase = ph;

    // Modulate cutoff frequency

    float magnitude = envelopeBuffer.getMagnitude(0, 0, numSamples);
    float modCutoff = *filterParameters.cutoffParameter + ((magnitude * magnitude) * _cutOffDepth);

    // Apply low pass filter

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    _ladderFilter.setCutoffFrequencyHz(modCutoff);
    _ladderFilter.process(context);

    // Mix dry and wet signals
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *drySignal = dryBuffer.getReadPointer(channel);
        auto *wetSignal = buffer.getWritePointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float in = drySignal[i];
            // Mix dry and wet signals
            wetSignal[i] = (1.0f - *mixerParameters.mixParameter) * in + *mixerParameters.mixParameter * wetSignal[i];
        }
    }
}

//==============================================================================
bool MelloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *MelloAudioProcessor::createEditor()
{
    return new MelloAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void MelloAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MelloAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MelloAudioProcessor();
}

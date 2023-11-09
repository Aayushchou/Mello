#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

//==============================================================================
MelloAudioProcessor::MelloAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
}

MelloAudioProcessor::~MelloAudioProcessor()
{
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

float MelloAudioProcessor::lfo(float phase, float envelopeDepth)
{
    // The envelopeDepth parameter is expected to be in the range [0, 1].
    // If envelopeDepth is 0, the LFO effect is fully off.
    // If envelopeDepth is 1, the LFO has full depth.

    // Return the LFO value with the modulated depth
    float normalizedDepth = _delayMin + envelopeDepth * (_delayDiff);

    float lfoValue = 0.05 * (0.5 + 0.5 * sinf(2.0 * M_PI * phase));

    return lfoValue;
}

//==============================================================================
void MelloAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    _cutOffDepth = (getSampleRate() * 0.5);
    _phase = 0.0;
    _delayMin = 0.001f;
    _delayMax = 0.01f;
    int maxDelayInSamples = static_cast<int>(std::round(_delayMax * getSampleRate()));
    _delayDiff = _delayMax - _delayMin;
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
    _ladderFilter.setDrive(1.0);

    // set up ballistic filter
    _ballisticsFilter.prepare(spec);
    _ballisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::peak);
    _ballisticsFilter.setAttackTime(0.1);
    _ballisticsFilter.setReleaseTime(0.2);

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
            ph += 3.0 * _invSampleRate;
            if (ph >= 1.0)
            {
                ph -= 1.0;
            }
        }
    }

    _phase = ph;

    // Modulate cutoff frequency

    float magnitude = envelopeBuffer.getMagnitude(0, 0, numSamples);
    float modCutoff = _cutOff + ((magnitude * magnitude) * _cutOffDepth);

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
            wetSignal[i] = (1.0f - _dryMix) * in + _dryMix * wetSignal[i];
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
    return new MelloAudioProcessorEditor(*this);
}

//==============================================================================
void MelloAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void MelloAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MelloAudioProcessor();
}

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

namespace ParameterID
{
#define PARAMETER_ID(str) constexpr const char *str{#str};

    PARAMETER_ID(kEnvelopeAttack)
    PARAMETER_ID(kEnvelopeRelease)
    PARAMETER_ID(kEnvelopeMapping)
    PARAMETER_ID(kVibratoMin)
    PARAMETER_ID(kVibratoMax)
    PARAMETER_ID(kVibratoRate)
    PARAMETER_ID(kLowOrBand)
    PARAMETER_ID(kLowPassCutoff)
    PARAMETER_ID(kLowPassResonance)
    PARAMETER_ID(kLowPassDrive)
    PARAMETER_ID(kMix)
}

struct EnvelopeParameters
{
    std::atomic<float> *attackParameter = nullptr;
    std::atomic<float> *releaseParameter = nullptr;
    std::atomic<float> *mappingParameter = nullptr;
};

struct VibratoParameters
{
    std::atomic<float> *minDepthParameter = nullptr;
    std::atomic<float> *maxDepthParameter = nullptr;
    std::atomic<float> *rateParameter = nullptr;
};

struct FilterParameters
{
    std::atomic<float> *lowOrBandParameter = nullptr;
    std::atomic<float> *cutoffParameter = nullptr;
    std::atomic<float> *resonanceParameter = nullptr;
    std::atomic<float> *driveParameter = nullptr;
};

struct MixerParameters
{
    std::atomic<float> *mixParameter = nullptr;
};

//==============================================================================
class MelloAudioProcessor final : public juce::AudioProcessor, private juce::ValueTree::Listener
{
public:
    float _dryMix;
    float _cutOff;

    //==============================================================================
    MelloAudioProcessor();
    ~MelloAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    float _cutOffDepth;
    float _delayMin;
    float _delayMax;
    float _delayDiff;
    float _invSampleRate;
    float _phase;

    EnvelopeParameters envelopeParameters;
    VibratoParameters vibratoParameters;
    FilterParameters filterParameters;
    MixerParameters mixerParameters;

    juce::AudioProcessorValueTreeState parameters;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::dsp::LadderFilter<float> _ladderFilter;
    juce::dsp::BallisticsFilter<float> _ballisticsFilter;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> _delayLine;

    juce::AudioBuffer<float> dryBuffer;
    juce::AudioBuffer<float> envelopeBuffer;
    juce::AudioBuffer<float> delayBuffer;

    int getRandomInteger(int min, int max);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessor)
};

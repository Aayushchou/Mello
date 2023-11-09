#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//==============================================================================
class MelloAudioProcessor final : public juce::AudioProcessor
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
    juce::dsp::LadderFilter<float> _ladderFilter;
    juce::dsp::BallisticsFilter<float> _ballisticsFilter;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> _delayLine;

    juce::AudioBuffer<float> dryBuffer;
    juce::AudioBuffer<float> envelopeBuffer;
    juce::AudioBuffer<float> delayBuffer;

    int getRandomInteger(int min, int max);
    float lfo(float frequency, float envelopeDepth);
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessor)
};

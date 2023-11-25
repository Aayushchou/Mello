#pragma once

#include "PluginProcessor.h"
#include "SliderComponent.h"
#include "LevelMeter.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
class MelloAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit MelloAudioProcessorEditor(MelloAudioProcessor &, juce::AudioProcessorValueTreeState &);
    ~MelloAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    bool isResizable();

    SliderComponent sMix;
    SliderComponent sCutoff;
    SliderComponent sResonance;
    SliderComponent sDrive;
    SliderComponent sVibMin;
    SliderComponent sVibMax;
    SliderComponent sVibRate;
    SliderComponent sAttack;
    SliderComponent sRelease;

    LevelMeter leftMeter;
    MelloAudioProcessor &processorRef;
    juce::AudioProcessorValueTreeState &processorState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessorEditor)
};

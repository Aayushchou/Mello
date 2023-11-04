#pragma once

#include "PluginProcessor.h"

//==============================================================================
class MelloAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MelloAudioProcessorEditor(MelloAudioProcessor &);
    ~MelloAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MelloAudioProcessor &processorRef;

    juce::Slider _dryWetRatio;
    juce::Slider _lowPassGate;
    juce::Slider _depth;
    juce::Slider _damp;
    juce::Slider _rate;
    juce::Slider _resonance;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessorEditor)
};

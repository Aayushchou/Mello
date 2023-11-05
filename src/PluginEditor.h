#pragma once

#include "PluginProcessor.h"

//==============================================================================
class MelloAudioProcessorEditor final : public juce::AudioProcessorEditor, private juce::Slider::Listener
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

    void sliderValueChanged(juce::Slider *slider) override;
    MelloAudioProcessor &processorRef;

    juce::Slider _drySlider;
    juce::Slider _lpgSlider;
    juce::Slider _depthSlider;
    juce::Slider _dampSlider;
    juce::Slider _rateSlider;
    juce::Slider _resonanceSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessorEditor)
};

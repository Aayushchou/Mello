#pragma once

#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

class SliderComponent : public juce::Component
{
public:
    explicit SliderComponent(juce::String, juce::String, juce::AudioProcessorValueTreeState &);
    ~SliderComponent() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    juce::Slider slider;
    juce::Label label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderComponent)
};
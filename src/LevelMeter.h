#pragma once

#include "PluginProcessor.h"

class LevelMeter : public juce::Component
{
public:
    LevelMeter();
    ~LevelMeter() override;

    void paint(juce::Graphics &) override;
    void setLevel(const float value);

private:
    float level = -60.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};
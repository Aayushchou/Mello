#include "LevelMeter.h"

LevelMeter::LevelMeter() {}

LevelMeter::~LevelMeter() {}

void LevelMeter::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colours::white.withBrightness(0.4f));
    g.fillRoundedRectangle(bounds, 5.f);

    g.setColour(juce::Colours::white);
    const auto scaledY = juce::jmap(level, -60.f, +6.f, 0.f, static_cast<float>(getHeight()));
    g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.f);
}

void LevelMeter::setLevel(const float value)
{
    level = value;
}
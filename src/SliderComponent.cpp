#include "SliderComponent.h"

SliderComponent::SliderComponent(juce::String paramID, juce::String labelText, juce::AudioProcessorValueTreeState &vts)
{
    label.setText(labelText, juce::dontSendNotification);
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, slider.getTextBoxWidth(), slider.getTextBoxHeight());
    addAndMakeVisible(&label);
    addAndMakeVisible(&slider);
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, paramID, slider));
}

SliderComponent::~SliderComponent()
{
}

void SliderComponent::paint(juce::Graphics &g)
{
}

void SliderComponent::resized()
{
    auto area = getLocalBounds(); // Get the local bounds of the component

    // Define the height for the label and adjust font size
    auto labelHeight = 20;            // Adjust the height as needed
    label.setFont(juce::Font(14.0f)); // Set font size to 10, adjust as needed

    // Add some empty space above the label
    int labelTopPadding = 10; // Adjust the top padding as needed
    auto labelArea = area.removeFromTop(labelHeight + labelTopPadding);

    // Set the area for the label, considering the padding
    label.setBounds(labelArea.withTrimmedTop(labelTopPadding));
    label.setJustificationType(juce::Justification::centredBottom);

    // Add some margin between label and slider (optional)
    int margin = 5;
    area.removeFromTop(margin);

    // Set the bounds for the slider in the remaining area
    slider.setBounds(area);
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

SliderComponent::SliderComponent(juce::String paramID, juce::String labelText, juce::AudioProcessorValueTreeState &vts)
{
    label.setText(labelText, juce::dontSendNotification);
    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 45, 0);
    slider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&label);
    addAndMakeVisible(&slider);
    attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, paramID, slider));
}

SliderComponent::~SliderComponent()
{
}

void SliderComponent::paint(juce::Graphics &)
{
}

void SliderComponent::resized()
{
    auto area = getLocalBounds(); // Get the local bounds of the component

    // Define the height for the label and the margin space for the hover text box
    auto labelHeight = area.getHeight() / 4; // Allocate a portion of the height to the label
    auto margin = 10;                        // Space for the hover text box

    // Adjust the area to leave space at the bottom for the hover text box
    area.reduce(0, margin);

    // Set the label bounds to the top portion and center it horizontally
    auto labelArea = area.removeFromTop(labelHeight);
    label.setBounds(labelArea.reduced(labelArea.getWidth() / 4, 0));

    // Now the area variable has been reduced from the top by the height of the label
    slider.setBounds(area); // The remaining area is for the slider
}

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p),
      processorRef(p),
      processorState(vts),
      sMix(ParameterID::kMix, "Mix", vts)
{
    juce::ignoreUnused(processorRef);

    addAndMakeVisible(sMix);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(530, 300);
    setResizable(true, true);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

bool MelloAudioProcessorEditor::isResizable() { return true; }

//==============================================================================
void MelloAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(15.0f);
    g.setOpacity(0.8f);
}

void MelloAudioProcessorEditor::resized()
{

    sMix.setBounds(100, 100, getWidth() * 0.3, getHeight() * 0.3);
}
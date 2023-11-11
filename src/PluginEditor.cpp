#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), processorRef(p), processorState(vts)
{
    juce::ignoreUnused(processorRef);

    for (auto &config : sliderConfigs)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(530, 300);
    setResizable(true, false);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

bool MelloAudioProcessorEditor::isResizable() { return true; }

void MelloAudioProcessorEditor::createSlider(juce::Component &target,
                                             juce::Slider *slider,
                                             std::unique_ptr<SliderAttachment> *attachment,
                                             juce::Label *label,
                                             const juce::String labelText,
                                             const juce::String paramID)
{
    label->setText(labelText, juce::dontSendNotification);
    label->attachToComponent(slider, true);
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    target.addAndMakeVisible(*label);
    target.addAndMakeVisible(*slider);
    attachment->reset(new SliderAttachment(processorState, paramID, *slider));
}

void MelloAudioProcessorEditor::createToggle(juce::Component &target,
                                             juce::ToggleButton &button,
                                             std::unique_ptr<ButtonAttachment> &attachment,
                                             const juce::String &buttonText,
                                             const juce::String &paramID)
{

    button.setButtonText(buttonText);
    target.addAndMakeVisible(button);
    attachment.reset(new ButtonAttachment(processorState, paramID, button));
}

void MelloAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
}

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
    // Define the flexbox layout
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;          // Arrange components in rows
    fb.flexWrap = juce::FlexBox::Wrap::wrap;                   // Allow wrapping onto new lines
    fb.justifyContent = juce::FlexBox::JustifyContent::center; // Center items on the main-axis
    fb.alignContent = juce::FlexBox::AlignContent::stretch;    // Stretch items to fill the cross-axis

    // Define the dimensions of sliders based on the editor size
    const float sliderWidth = getWidth() / 3.0f - 20.0f;   // Divide width into 3 columns
    const float sliderHeight = getHeight() / 3.0f - 20.0f; // Divide height into 3 rows

    // Add sliders to the flexbox
    for (auto &config : sliderConfigs)
    {
        // Set up flex item properties
        juce::FlexItem flexItem(sliderWidth, sliderHeight);
        flexItem.margin = 5;                          // Set margin around items
        flexItem.associatedComponent = config.slider; // Associate the slider with the flex item
        fb.items.add(flexItem);

        // Adjust label positions
        config.label->setBounds(config.slider->getX(), config.slider->getY() - 20, sliderWidth, 20);
    }

    // Perform layout calculation
    fb.performLayout(getLocalBounds().reduced(10)); // Apply the layout within the editor bounds

    // Now manually set the bounds for each label since they are not part of the FlexBox items
    for (auto &config : sliderConfigs)
    {
        auto sliderBounds = config.slider->getBounds();
        config.label->setBounds(sliderBounds.getX(), sliderBounds.getY() - 20, sliderBounds.getWidth(), 20);
    }
}

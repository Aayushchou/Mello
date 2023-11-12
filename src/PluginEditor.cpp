#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), processorRef(p), processorState(vts)
{
    juce::ignoreUnused(processorRef);

    // Define the style and colors for the sliders and labels
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::coral);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightgrey);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::white);

    for (auto &config : sliderConfigs)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }

    setSize(530, 300);
    setResizable(true, true);
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
    // Slider style
    slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 0);
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::transparentWhite);
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::coral);

    // Label style
    label->setFont(juce::Font(15.0f, juce::Font::bold));
    label->setJustificationType(juce::Justification::centredBottom);
    label->attachToComponent(slider, false);

    // Adding components
    target.addAndMakeVisible(*label);
    target.addAndMakeVisible(*slider);

    // Create and configure the attachment
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
    // Use a FlexBox container for the sliders
    juce::FlexBox flexBox;
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    flexBox.alignContent = juce::FlexBox::AlignContent::stretch;
    flexBox.alignItems = juce::FlexBox::AlignItems::center;

    // Calculate the slider size based on the number of sliders
    const int numberOfSliders = 9;                                             // Replace this with the actual number of sliders if it's constant
    const float sliderWidth = getWidth() / 3.0f - 20.0f;                       // Three columns layout
    const float sliderHeight = getHeight() / (numberOfSliders / 3.0f) - 20.0f; // Rows based on number of sliders and columns

    // Add flex items for each slider
    for (auto &config : sliderConfigs)
    {
        juce::FlexItem flexItem(sliderWidth, sliderHeight, *config.slider);
        flexItem.margin = juce::FlexItem::Margin(10);
        flexBox.items.add(flexItem);
    }

    // Layout the flexbox within the editor bounds
    flexBox.performLayout(getLocalBounds());

    // Adjust label bounds
    for (auto &config : sliderConfigs)
    {
        auto sliderBounds = config.slider->getBounds();
        config.label->setBounds(sliderBounds.getX(), sliderBounds.getBottom() + 5, sliderBounds.getWidth(), 20);
    }
}

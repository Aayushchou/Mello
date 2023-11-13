#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p), processorRef(p), processorState(vts)
{
    juce::ignoreUnused(processorRef);

    for (auto &config : envelopeConfig)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
    for (auto &config : vibratoConfig)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
    for (auto &config : filterConfig)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
    for (auto &config : mixConfig)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(730, 300);
    setResizable(true, true);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

bool MelloAudioProcessorEditor::isResizable() { return true; }

void MelloAudioProcessorEditor::createSlidersForConfig(const std::vector<SliderConfig> &configs)
{
    for (auto &config : configs)
    {
        createSlider(*this, config.slider, config.attachment, config.label, config.labelText, config.paramID);
    }
}

void MelloAudioProcessorEditor::createSlider(juce::Component &target,
                                             juce::Slider *slider,
                                             std::unique_ptr<SliderAttachment> *attachment,
                                             juce::Label *label,
                                             const juce::String labelText,
                                             const juce::String paramID)
{
    label->setText(labelText, juce::dontSendNotification);
    label->attachToComponent(slider, false);
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 45, 0);
    slider->setPopupDisplayEnabled(true, true, &target);
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
    // Define the dimensions of sliders based on the editor size
    const float sliderWidth = getWidth() / 4.0f;   // Divide width into 3 columns
    const float sliderHeight = getHeight() / 4.0f; // Divide height into 3 rows

    auto area = getLocalBounds();
    auto envelopeArea = area.removeFromLeft(getWidth() * 0.55);
    // The remaining half is split into three equal parts for vibratoArea, filterArea, and mixArea
    auto remainingArea = area; // this is the remaining half after removing envelopeArea
    auto vibratoArea = remainingArea.removeFromLeft(remainingArea.getWidth() / 3);
    auto filterArea = remainingArea.removeFromLeft(remainingArea.getWidth() / 2); // now remainingArea is halved
    auto mixArea = remainingArea;                                                 // the remaining third is for the mixArea

    // Define the flexbox layout
    juce::FlexBox envelopeFb;
    envelopeFb.flexDirection = juce::FlexBox::Direction::row;             // Arrange components in rows
    envelopeFb.flexWrap = juce::FlexBox::Wrap::wrap;                      // Allow wrapping onto new lines
    envelopeFb.justifyContent = juce::FlexBox::JustifyContent::flexStart; // Center items on the main-axis
    envelopeFb.alignContent = juce::FlexBox::AlignContent::flexEnd;       // Stretch items to fill the cross-axis

    // Define the flexbox layout
    juce::FlexBox vibratoFb;
    vibratoFb.flexDirection = juce::FlexBox::Direction::column;       // Arrange components in rows
    vibratoFb.flexWrap = juce::FlexBox::Wrap::wrap;                   // Allow wrapping onto new lines
    vibratoFb.justifyContent = juce::FlexBox::JustifyContent::center; // Center items on the main-axis
    vibratoFb.alignContent = juce::FlexBox::AlignContent::center;     // Stretch items to fill the cross-axis

    // Define the flexbox layout
    juce::FlexBox filterFb;
    filterFb.flexDirection = juce::FlexBox::Direction::column;       // Arrange components in rows
    filterFb.flexWrap = juce::FlexBox::Wrap::wrap;                   // Allow wrapping onto new lines
    filterFb.justifyContent = juce::FlexBox::JustifyContent::center; // Center items on the main-axis
    filterFb.alignContent = juce::FlexBox::AlignContent::center;     // Stretch items to fill the cross-axis

    // Define the flexbox layout
    juce::FlexBox mixFb;
    mixFb.flexDirection = juce::FlexBox::Direction::column;        // Arrange components in rows
    mixFb.flexWrap = juce::FlexBox::Wrap::wrap;                    // Allow wrapping onto new lines
    mixFb.justifyContent = juce::FlexBox::JustifyContent::flexEnd; // Center items on the main-axis
    mixFb.alignContent = juce::FlexBox::AlignContent::flexEnd;     // Stretch items to fill the cross-axis
    // Add sliders to the flexbox
    for (auto &config : envelopeConfig)
    {
        // Set up flex item properties
        juce::FlexItem envItem(sliderWidth, sliderHeight);
        envItem.margin = 3;                          // Set margin around items
        envItem.associatedComponent = config.slider; // Associate the slider with the flex item
        envelopeFb.items.add(envItem);

        // Adjust label positions
        config.label->setBounds(config.slider->getX(), config.slider->getY() - 10, sliderWidth, 10);
    }
    // Perform layout calculation
    envelopeFb.performLayout(envelopeArea); // Apply the layout within the editor bounds

    for (auto &config : vibratoConfig)
    {
        // Set up flex item properties
        juce::FlexItem vibItem(sliderWidth, sliderHeight);
        vibItem.margin = 5;                          // Set margin around items
        vibItem.associatedComponent = config.slider; // Associate the slider with the flex item
        vibratoFb.items.add(vibItem);

        // Adjust label positions
        config.label->setBounds(config.slider->getX(), config.slider->getY() - 10, sliderWidth, 10);
    }
    // Perform layout calculation
    vibratoFb.performLayout(vibratoArea); // Apply the layout within the editor bounds

    for (auto &config : filterConfig)
    {
        // Set up flex item properties
        juce::FlexItem filterItem(sliderWidth, sliderHeight);
        filterItem.margin = 5;                          // Set margin around items
        filterItem.associatedComponent = config.slider; // Associate the slider with the flex item
        filterFb.items.add(filterItem);

        // Adjust label positions
        config.label->setBounds(config.slider->getX(), config.slider->getY() - 10, sliderWidth, 10);
    }
    // Perform layout calculation
    filterFb.performLayout(filterArea); // Apply the layout within the editor bounds

    for (auto &config : mixConfig)
    {
        // Set up flex item properties
        juce::FlexItem mixItem(sliderWidth, sliderHeight);
        mixItem.margin = 5;                          // Set margin around items
        mixItem.associatedComponent = config.slider; // Associate the slider with the flex item
        mixFb.items.add(mixItem);

        // Adjust label positions
        config.label->setBounds(config.slider->getX() + 20, config.slider->getY() - 10, sliderWidth, 10);
    }
    // Perform layout calculation
    mixFb.performLayout(mixArea); // Apply the layout within the editor bounds
}
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
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

void MelloAudioProcessorEditor::createSlider(juce::Component &target,
                                             juce::Slider *slider,
                                             std::unique_ptr<SliderAttachment> *attachment,
                                             juce::Label *label,
                                             const juce::String labelText,
                                             const juce::String paramID)
{

    label->setText(labelText, juce::dontSendNotification);
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
    if (slider == &_drySlider)
    {
        processorRef._dryMix = (float)_drySlider.getValue();
    }
    if (slider == &_lpgSlider)
    {
        processorRef._cutOff = (float)_lpgSlider.getValue();
    }
}

//==============================================================================
void MelloAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.setOpacity(1.0f);
}

void MelloAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    mixSlider.setBounds(20, 30, 150, 100);
    cutoffSlider.setBounds(20, 160, 150, 100);
    vibratoMaxSlider.setBounds(190, 30, 150, 100);
    vibratoMinSlider.setBounds(190, 160, 150, 100);
    rateSlider.setBounds(360, 30, 150, 100);
    driveSlider.setBounds(360, 160, 150, 100);
}

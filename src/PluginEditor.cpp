#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(&_drySlider);
    _drySlider.setSliderStyle(juce::Slider::Rotary);
    _drySlider.setRange(0.0, 1.0, 0.01);
    _drySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _drySlider.setPopupDisplayEnabled(true, false, this);
    _drySlider.setValue(0.5);
    _drySlider.addListener(this);

    addAndMakeVisible(&_lpgSlider);
    _lpgSlider.setSliderStyle(juce::Slider::Rotary);
    _lpgSlider.setRange(0.0, 1.0, 0.01);
    _lpgSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _lpgSlider.setPopupDisplayEnabled(true, false, this);
    _lpgSlider.setValue(0.5);

    addAndMakeVisible(&_depthSlider);
    _depthSlider.setSliderStyle(juce::Slider::Rotary);
    _depthSlider.setRange(0.0, 1.0, 0.01);
    _depthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _depthSlider.setPopupDisplayEnabled(true, false, this);
    _depthSlider.setValue(0.5);

    addAndMakeVisible(&_dampSlider);
    _dampSlider.setSliderStyle(juce::Slider::Rotary);
    _dampSlider.setRange(0.0, 1.0, 0.01);
    _dampSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _dampSlider.setPopupDisplayEnabled(true, false, this);
    _dampSlider.setValue(0.5);

    addAndMakeVisible(&_rateSlider);
    _rateSlider.setSliderStyle(juce::Slider::Rotary);
    _rateSlider.setRange(0.0, 1.0, 0.01);
    _rateSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _rateSlider.setPopupDisplayEnabled(true, false, this);
    _rateSlider.setValue(0.5);

    addAndMakeVisible(&_resonanceSlider);
    _resonanceSlider.setSliderStyle(juce::Slider::Rotary);
    _resonanceSlider.setRange(0.0, 1.0, 0.01);
    _resonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _resonanceSlider.setPopupDisplayEnabled(true, false, this);
    _resonanceSlider.setValue(0.5);

    setSize(530, 300);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

void MelloAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &_drySlider)
    {
        processorRef._dryMix = (float)_drySlider.getValue();
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
    _drySlider.setBounds(20, 30, 150, 100);
    _lpgSlider.setBounds(20, 160, 150, 100);
    _depthSlider.setBounds(190, 30, 150, 100);
    _dampSlider.setBounds(190, 160, 150, 100);
    _rateSlider.setBounds(360, 30, 150, 100);
    _resonanceSlider.setBounds(360, 160, 150, 100);
}

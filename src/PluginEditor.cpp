#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(&_dryWetRatio);
    _dryWetRatio.setSliderStyle(juce::Slider::Rotary);
    _dryWetRatio.setRange(0.0, 1.0, 0.01);
    _dryWetRatio.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _dryWetRatio.setPopupDisplayEnabled(true, false, this);
    _dryWetRatio.setValue(0.5);

    addAndMakeVisible(&_lowPassGate);
    _lowPassGate.setSliderStyle(juce::Slider::Rotary);
    _lowPassGate.setRange(0.0, 1.0, 0.01);
    _lowPassGate.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _lowPassGate.setPopupDisplayEnabled(true, false, this);
    _lowPassGate.setValue(0.5);

    addAndMakeVisible(&_depth);
    _depth.setSliderStyle(juce::Slider::Rotary);
    _depth.setRange(0.0, 1.0, 0.01);
    _depth.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _depth.setPopupDisplayEnabled(true, false, this);
    _depth.setValue(0.5);

    addAndMakeVisible(&_damp);
    _damp.setSliderStyle(juce::Slider::Rotary);
    _damp.setRange(0.0, 1.0, 0.01);
    _damp.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _damp.setPopupDisplayEnabled(true, false, this);
    _damp.setValue(0.5);

    addAndMakeVisible(&_rate);
    _rate.setSliderStyle(juce::Slider::Rotary);
    _rate.setRange(0.0, 1.0, 0.01);
    _rate.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _rate.setPopupDisplayEnabled(true, false, this);
    _rate.setValue(0.5);

    addAndMakeVisible(&_resonance);
    _resonance.setSliderStyle(juce::Slider::Rotary);
    _resonance.setRange(0.0, 1.0, 0.01);
    _resonance.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    _resonance.setPopupDisplayEnabled(true, false, this);
    _resonance.setValue(0.5);

    setSize(530, 300);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
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
    _dryWetRatio.setBounds(20, 30, 150, 100);
    _lowPassGate.setBounds(20, 160, 150, 100);
    _depth.setBounds(190, 30, 150, 100);
    _damp.setBounds(190, 160, 150, 100);
    _rate.setBounds(360, 30, 150, 100);
    _resonance.setBounds(360, 160, 150, 100);
}

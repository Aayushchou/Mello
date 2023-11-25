#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MelloAudioProcessorEditor::MelloAudioProcessorEditor(MelloAudioProcessor &p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor(&p),
      processorRef(p),
      processorState(vts),
      sMix(ParameterID::kMix, "Mix", vts),
      sCutoff(ParameterID::kLowPassCutoff, "Cutoff", vts),
      sResonance(ParameterID::kLowPassResonance, "Resonance", vts),
      sDrive(ParameterID::kLowPassDrive, "Drive", vts),
      sVibMin(ParameterID::kVibratoMin, "Min", vts),
      sVibMax(ParameterID::kVibratoMax, "Max", vts),
      sVibRate(ParameterID::kVibratoRate, "Rate", vts),
      sAttack(ParameterID::kEnvelopeAttack, "Attack", vts),
      sRelease(ParameterID::kEnvelopeRelease, "Release", vts)
{
    juce::ignoreUnused(processorRef);

    addAndMakeVisible(sMix);
    addAndMakeVisible(sCutoff);
    addAndMakeVisible(sResonance);
    addAndMakeVisible(sDrive);
    addAndMakeVisible(sVibMin);
    addAndMakeVisible(sVibMax);
    addAndMakeVisible(sVibRate);
    addAndMakeVisible(sAttack);
    addAndMakeVisible(sRelease);
    addAndMakeVisible(leftMeter);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(730, 330);
    setResizable(true, true);
    startTimerHz(24);
}

MelloAudioProcessorEditor::~MelloAudioProcessorEditor()
{
}

bool MelloAudioProcessorEditor::isResizable() { return true; }

void MelloAudioProcessorEditor::timerCallback()
{
    leftMeter.setLevel(processorRef.getRMSValue(0));
    leftMeter.repaint();
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
    auto area = getLocalBounds();

    // Divide the area into left and right halves
    auto leftArea = area.removeFromLeft(area.getWidth() / 2);
    auto rightArea = area;

    // Set bounds for the Attack and Release sliders in the bottom-left quarter
    auto bottomLeftArea = leftArea.removeFromBottom(leftArea.getHeight() / 2.5);
    sVibMin.setBounds(bottomLeftArea.removeFromLeft(bottomLeftArea.getWidth() / 2));
    sVibMax.setBounds(bottomLeftArea);

    // Divide the right half into three vertical strips
    auto stripWidth = rightArea.getWidth() / 3;
    auto strip1Area = rightArea.removeFromLeft(stripWidth);
    auto strip2Area = rightArea.removeFromLeft(stripWidth);
    auto strip3Area = rightArea; // Remaining area for the third strip
    auto mixArea = strip3Area.removeFromBottom(strip3Area.getHeight() / 3);

    // Set bounds for the Vibrato sliders in the first strip
    auto vibSliderHeight = strip1Area.getHeight() / 3;
    sAttack.setBounds(strip1Area.removeFromTop(vibSliderHeight));
    sRelease.setBounds(strip1Area.removeFromTop(vibSliderHeight));
    sVibRate.setBounds(strip1Area);

    // Set bounds for the Low Pass Filter sliders in the second strip
    auto filterSliderHeight = strip2Area.getHeight() / 3;
    sCutoff.setBounds(strip2Area.removeFromTop(filterSliderHeight));
    sResonance.setBounds(strip2Area.removeFromTop(filterSliderHeight));
    sDrive.setBounds(strip2Area);

    // Set bounds for the Mix slider at the top of the third strip
    sMix.setBounds(mixArea);

    const int levelMeterWidth = strip3Area.getWidth() / 4;
    const int topPadding = 10;
    const int bottomPadding = 10;
    auto levelArea = strip3Area.withTrimmedLeft((strip3Area.getWidth() - levelMeterWidth) / 2)
                         .withTrimmedRight((strip3Area.getWidth() - levelMeterWidth) / 2)
                         .withTrimmedTop(topPadding)
                         .withTrimmedBottom(bottomPadding);

    leftMeter.setBounds(levelArea);
}

#pragma once

#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

struct SliderConfig
{
    juce::Slider *slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> *attachment;
    juce::Label *label;
    juce::String labelText;
    juce::String paramID;
};

//==============================================================================
class MelloAudioProcessorEditor final : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    explicit MelloAudioProcessorEditor(MelloAudioProcessor &, juce::AudioProcessorValueTreeState &);
    ~MelloAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void createSlider(juce::Component &target,
                      juce::Slider *slider,
                      std::unique_ptr<SliderAttachment> *attachment,
                      juce::Label *label,
                      const juce::String labelText,
                      const juce::String paramID);

    void createToggle(juce::Component &target,
                      juce::ToggleButton &button,
                      std::unique_ptr<ButtonAttachment> &attachment,
                      const juce::String &buttonText,
                      const juce::String &paramID);

    void sliderValueChanged(juce::Slider *slider) override;

    bool isResizable();

    MelloAudioProcessor &processorRef;
    juce::AudioProcessorValueTreeState &processorState;

    // sliders
    juce::Slider attackSlider,
        releaseSlider,
        vibratoMinSlider,
        vibratoMaxSlider,
        rateSlider,
        cutoffSlider,
        resonanceSlider,
        driveSlider,
        mixSlider;

    juce::Label attackLabel,
        releaseLabel,
        vibratoMinLabel,
        vibratoMaxLabel,
        rateLabel,
        cutoffLabel,
        resonanceLabel,
        driveLabel,
        mixLabel;

    // toggles
    juce::ToggleButton mappingToggle, lowPassToggle;

    // attachments
    std::unique_ptr<SliderAttachment> attackAttachment,
        releaseAttachment,
        vibratoMinAttachment,
        vibratoMaxAttachment,
        rateAttachment,
        cutoffAttachment,
        resonanceAttachment,
        driveAttachment,
        mixAttachment;

    SliderConfig sliderConfigs[9] = {

        {&vibratoMinSlider, &vibratoMinAttachment, &vibratoMinLabel, "Vibrato Min", ParameterID::kVibratoMin},
        {&vibratoMaxSlider, &vibratoMaxAttachment, &vibratoMaxLabel, "Vibrato Max", ParameterID::kVibratoMax},
        {&rateSlider, &rateAttachment, &rateLabel, "Rate", ParameterID::kVibratoRate},
        {&cutoffSlider, &cutoffAttachment, &cutoffLabel, "Cutoff", ParameterID::kLowPassCutoff},
        {&driveSlider, &driveAttachment, &driveLabel, "Drive", ParameterID::kLowPassDrive},
        {&resonanceSlider, &resonanceAttachment, &resonanceLabel, "Resonance", ParameterID::kLowPassResonance},
        {&attackSlider, &attackAttachment, &attackLabel, "Attack", ParameterID::kEnvelopeAttack},
        {&releaseSlider, &releaseAttachment, &releaseLabel, "Release", ParameterID::kEnvelopeRelease},
        {&mixSlider, &mixAttachment, &mixLabel, "Mix", ParameterID::kMix}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MelloAudioProcessorEditor)
};

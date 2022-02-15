#pragma once

#include "PluginProcessor.h"
#include "RotaryDecibelSlider.h"

//==============================================================================
class SimplePluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &);
    ~SimplePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // juce::Label rotaryDecibelLabel;
    RotaryDecibelSlider gainKnob;
    juce::ToggleButton muteButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteButtonAttachment;

    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};


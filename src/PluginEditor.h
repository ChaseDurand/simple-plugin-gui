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
    juce::Label rotaryDecibelLabel;
    float level = 1.0f;
    RotaryDecibelSlider rotaryDecibelSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};


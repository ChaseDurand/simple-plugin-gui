#pragma once

#include "PluginProcessor.h"

//==============================================================================
class SimplePluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                         public juce::Slider::Listener
{
public:
    explicit SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &);
    ~SimplePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* slider) override;

private:
    juce::Slider mGainSlider;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};

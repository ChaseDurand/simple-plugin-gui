#pragma once

#include "PluginProcessor.h"
#include "RotaryDecibelSlider.h"
#include "MuteButton.h"
#include "HorizontalMeter.h"

//==============================================================================
class SimplePluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                         public juce::Timer
{
public:
    explicit SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &);
    ~SimplePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;
    void timerCallback() override;

private:
    // juce::Label rotaryDecibelLabel;
    RotaryDecibelSlider gainKnob;
    MuteButton muteButton;

    juce::ToggleButton buttonChannelL, buttonChannelC, buttonChannelR;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelButtonLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelButtonCAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelButtonRAttachment;

    HorizontalMeter meterL, meterR;

    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};


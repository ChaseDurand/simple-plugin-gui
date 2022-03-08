#pragma once

#include "PluginProcessor.h"
#include "GUI/RotaryDecibelSlider.h"
#include "GUI/MuteButton.h"
#include "GUI/ChannelButton.h"
#include "GUI/VerticalMeter.h"

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
    
    RotaryDecibelSlider gainKnob;
    MuteButton muteButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteButtonAttachment;
    std::unique_ptr<juce::ParameterAttachment> channelButtonAttachment;
    std::vector<std::unique_ptr<juce::Button>> channelButtons;

    VerticalMeter meterL, meterR;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};


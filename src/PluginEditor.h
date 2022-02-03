#pragma once

#include "PluginProcessor.h"

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

    class DecibelSlider : public juce::Slider{
        public:
            DecibelSlider() {}
            double getValueFromText (const juce::String& text) override {
                // Strip "db suffix and trim whitespace"
                auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();
                // If remaining text is "-inf", return minusInfinitydB, otherwise return value
                return decibelText.equalsIgnoreCase("-inf") ? NEGATIVE_INF_THRESH : decibelText.getDoubleValue();
                
            }

            juce::String getTextFromValue (double value) override {
                return (value <= NEGATIVE_INF_THRESH) ? "-inf" : juce::Decibels::toString (value);
            }
        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSlider)
    };

    
    juce::Label decibelLabel;
    float level = 1.0f;
    DecibelSlider decibelSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimplePluginAudioProcessor &processorRef;

public:
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sliderValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessorEditor)
};


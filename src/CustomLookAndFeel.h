#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class RotaryDecibelSliderLookAndFeel : public juce::LookAndFeel_V4{
    public:
        RotaryDecibelSliderLookAndFeel();

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;

    juce::Label* createSliderTextBox(juce::Slider& slider) override;

};
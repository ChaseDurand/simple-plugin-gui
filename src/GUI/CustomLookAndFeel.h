#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "ResizableLabel.h"

class RotaryDecibelSliderLookAndFeel : public juce::LookAndFeel_V4
{
    public:
        RotaryDecibelSliderLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;


    RisizableTextBox* createSliderTextBox(juce::Slider& slider) override;

};

class MuteButtonLookAndFeel : public juce::LookAndFeel_V4, juce::Component
{
    public:

    void drawTickBox (juce::Graphics& g, juce::Component& button, float x, float y,
        float w, float h, bool ticked, bool isEnabled,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown) override;
};


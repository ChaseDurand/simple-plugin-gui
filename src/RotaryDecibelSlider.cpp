#include "RotaryDecibelSlider.h"
#include "ComponentFocus.h"
#include "ResizableLabel.h"

RotaryDecibelSlider::RotaryDecibelSlider() : juce::Slider()
{
    setLookAndFeel(&rotaryDecibelSliderLookAndFeel);
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setWantsKeyboardFocus(true);
    setVelocityBasedMode (true);
    setRange(NEGATIVE_INF_THRESH, GAIN_MAX, 0.1f);
    setDoubleClickReturnValue(true, 0.0f);
}
RotaryDecibelSlider::~RotaryDecibelSlider()
{
    setLookAndFeel(nullptr);
}

void RotaryDecibelSlider::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);
    // If component has keyboard focus, draw selector indicators
    if(hasKeyboardFocus(true)){
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        drawFocusOutline(bounds, g);
    }
}

void RotaryDecibelSlider::mouseDown(const juce::MouseEvent& event)
{
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
    mousePosition = juce::Desktop::getMousePosition();
}

void RotaryDecibelSlider::mouseUp(const juce::MouseEvent& event)
{
    juce::Slider::mouseUp(event);
    juce::Desktop::setMousePosition(mousePosition);
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void RotaryDecibelSlider::mouseDrag(const juce::MouseEvent& event)
{
    // Increase sensitivity when shift is held
    juce::Slider::mouseDrag(event);
    if(event.mods.isShiftDown())
    {
        setVelocityModeParameters(0.1, 1, 0.1, false);
    }
    else
    {
        setVelocityModeParameters(1.0, 1, 0.1, false);
    }
}

double RotaryDecibelSlider::getValueFromText (const juce::String& text)
{
    // Strip "db suffix and trim whitespace"
    auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();
    // If remaining text is "-inf", return minusInfinitydB, otherwise return value
    return decibelText.equalsIgnoreCase("-inf") ? NEGATIVE_INF_THRESH : decibelText.getDoubleValue();
}

juce::String RotaryDecibelSlider::getTextFromValue (double value)
{
    return (value <= NEGATIVE_INF_THRESH) ? "-inf" : juce::Decibels::toString (value, 1);
}
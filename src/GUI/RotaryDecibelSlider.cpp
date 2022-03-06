#include "RotaryDecibelSlider.h"
#include "ComponentFocus.h"
#include "ResizableLabel.h"
#include "CustomColours.h"

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
        drawFocusOutline(bounds, g, CustomColours::offWhite);
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
        setVelocityModeParameters(0.08, 1, 0.1, false);
    }
    else
    {
        setVelocityModeParameters(0.8, 1, 0.1, false);
    }
}

bool RotaryDecibelSlider::keyPressed(const juce::KeyPress& key)
{
    // Incrase or decrease value on arrow key press.
    // If shift is held, shift by smaller increment.
    double increment = 1.0;
    if(juce::ModifierKeys::getCurrentModifiers().isShiftDown())
    {
        increment = 0.1;
    }
    if(key.isKeyCode(juce::KeyPress::upKey) || key.isKeyCode(juce::KeyPress::rightKey))
    
    {            
        // Increase value
        setValue(getValue()+increment);
        return true;

    }
    else if(key.isKeyCode(juce::KeyPress::downKey) || key.isKeyCode(juce::KeyPress::leftKey))
    {
        // Decrease value
        setValue(getValue()-increment);
        return true;
    }
    else
    {
        return false;
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
    return juce::Decibels::toString (value, 1, (double)NEGATIVE_INF_THRESH, true, "-inf");  
}
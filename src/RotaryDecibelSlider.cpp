#include "RotaryDecibelSlider.h"


RotaryDecibelSlider::RotaryDecibelSlider() : juce::Slider(){
}
RotaryDecibelSlider:: ~RotaryDecibelSlider() {
    setLookAndFeel(nullptr);
}

void RotaryDecibelSlider::mouseDown(const juce::MouseEvent& event){
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
    mousePosition = juce::Desktop::getMousePosition();
}

void RotaryDecibelSlider::mouseUp(const juce::MouseEvent& event){
    juce::Slider::mouseUp(event);
    juce::Desktop::setMousePosition(mousePosition);
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

double RotaryDecibelSlider::getValueFromText (const juce::String& text){
    // Strip "db suffix and trim whitespace"
    auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();
    // If remaining text is "-inf", return minusInfinitydB, otherwise return value
    return decibelText.equalsIgnoreCase("-inf") ? NEGATIVE_INF_THRESH : decibelText.getDoubleValue();
}

juce::String RotaryDecibelSlider::getTextFromValue (double value){
    return (value <= NEGATIVE_INF_THRESH) ? "-inf" : juce::Decibels::toString (value);
}
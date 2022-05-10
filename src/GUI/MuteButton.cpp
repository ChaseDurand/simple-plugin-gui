#include "MuteButton.h"
#include "ComponentFocus.h"
#include "CustomColours.h"

// TODO update graphics to match
// TODO scale font based on size

MuteButton::MuteButton() : juce::ToggleButton()
{
    setLookAndFeel(&muteButtonLookAndFeel);
    setWantsKeyboardFocus(true);
}

MuteButton::~MuteButton() {
    setLookAndFeel(nullptr);
}

void MuteButton::paint(juce::Graphics& g){
    juce::Button::paint(g);
    // If component has keyboard focus, draw selector indicators
    if(hasKeyboardFocus(true)){
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        drawFocusOutline(bounds, g, findColour(MuteButton::ColourIds::borderColourId), getTopLevelComponent()->getWidth());
    }
    else if(isMouseOver()){
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        drawFocusOutline(bounds, g, CustomColours::offWhite.withAlpha(0.5f), getTopLevelComponent()->getWidth());     
    }
}
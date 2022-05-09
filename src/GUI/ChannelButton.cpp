#include "ChannelButton.h"
#include "ComponentFocus.h"
#include "CustomColours.h"

ChannelButton::ChannelButton(const juce::String& buttonName) : juce::TextButton(buttonName)
{
    setLookAndFeel(&channelButtonLookAndFeel);
    setWantsKeyboardFocus(true);
}

ChannelButton::~ChannelButton()
{
    setLookAndFeel(nullptr);
}

// void ChannelButton::paint(juce::Graphics& g){
//     juce::Button::paint(g);
//     // If component has keyboard focus, draw selector indicators
//     if(hasKeyboardFocus(true)){
//         juce::Rectangle<float> bounds = getLocalBounds().toFloat();
//         drawFocusOutline(bounds, g, CustomColours::offWhite);
//     } 
// }
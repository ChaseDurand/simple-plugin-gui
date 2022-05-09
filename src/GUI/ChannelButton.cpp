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
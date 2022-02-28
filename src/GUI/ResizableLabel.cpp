#include "ResizableLabel.h"

juce::String RisizableTextBox::valueShownWithEditor = "";

RisizableTextBox::RisizableTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (false, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
}

void RisizableTextBox::resized()
{
    juce::Label::resized();
    setFont (getWidth() * 0.16f);
}
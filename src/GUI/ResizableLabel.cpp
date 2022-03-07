#include "ResizableLabel.h"

juce::String RisizableTextBox::valueShownWithEditor = "";

RisizableTextBox::RisizableTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (true, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::black);
}

void RisizableTextBox::resized()
{
    juce::Label::resized();
    setFont (getHeight());
}
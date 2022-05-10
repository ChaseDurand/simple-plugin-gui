#include "ResizableLabel.h"
#include "ComponentFocus.h"
#include "CustomColours.h"

// TODO update appearance when editing

juce::String ResizableTextBox::valueShownWithEditor = "";

ResizableTextBox::ResizableTextBox()
{
    setJustificationType (juce::Justification::centred);
    setInterceptsMouseClicks (true, false);
    setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::black);
}

void ResizableTextBox::resized()
{
    juce::Label::resized();
    setFont (getHeight());
}

void ResizableTextBox::paint(juce::Graphics &g)
{
    if(isMouseOver()){
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        drawFocusOutline(bounds, g, CustomColours::offWhite.withAlpha(0.5f), getTopLevelComponent()->getWidth());     
    }
    getLookAndFeel().drawLabel (g, *this);
}
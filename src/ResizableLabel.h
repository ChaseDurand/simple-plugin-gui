#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class RisizableTextBox : public juce::Label
{
public:
    static juce::String valueShownWithEditor;

    RisizableTextBox();

    void resized() override;
};
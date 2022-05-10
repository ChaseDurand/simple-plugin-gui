#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class ResizableTextBox : public juce::Label
{
public:
    static juce::String valueShownWithEditor;

    ResizableTextBox();

    void resized() override;
    void paint(juce::Graphics &g) override;
};
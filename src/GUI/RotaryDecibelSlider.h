#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "CustomLookAndFeel.h"
#include "../Config.h"

class RotaryDecibelSlider : public juce::Slider{
    public:
        RotaryDecibelSlider();
        ~RotaryDecibelSlider() override;

        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        bool keyPressed(const juce::KeyPress& key) override;
        double getValueFromText(const juce::String& text) override;
        juce::String getTextFromValue(double value) override;
    private:
        RotaryDecibelSliderLookAndFeel rotaryDecibelSliderLookAndFeel;
        juce::Point<int> mousePosition;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryDecibelSlider)
};
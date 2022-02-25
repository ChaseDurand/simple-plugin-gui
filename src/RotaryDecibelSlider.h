#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "CustomLookAndFeel.h"
#define NEGATIVE_INF_THRESH -60.0f // db theshold for silence
#define GAIN_MAX 24.0f

class RotaryDecibelSlider : public juce::Slider{
    public:
        RotaryDecibelSlider();
        ~RotaryDecibelSlider() override;

        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        double getValueFromText(const juce::String& text) override;
        juce::String getTextFromValue(double value) override;
    private:
        RotaryDecibelSliderLookAndFeel rotaryDecibelSliderLookAndFeel;
        juce::Point<int> mousePosition;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryDecibelSlider)
};
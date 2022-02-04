#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_basics/juce_audio_basics.h>
#define NEGATIVE_INF_THRESH -60.0f // db theshold for silence

class RotaryDecibelSlider : public juce::Slider{
    public:
        RotaryDecibelSlider();
        ~RotaryDecibelSlider() override;

        void mouseDown(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        double getValueFromText (const juce::String& text) override;
        juce::String getTextFromValue (double value) override;
    private:
        juce::Point<int> mousePosition;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotaryDecibelSlider)
};
#include <juce_gui_basics/juce_gui_basics.h>

class HorizontalMeter : public juce::Component{
    public:
        void paint(juce::Graphics& g) override{
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colour(127, 127, 127));
            g.fillRoundedRectangle(bounds, 5.f);

            g.setColour(juce::Colours::white);

            const auto scaledX = juce::jmap(level, -60.f, 6.f, 0.f , (float)getWidth());
            g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
        }

        void setLevel(const float value) {level = value; }
    private:
        float level = -60.0f;
};
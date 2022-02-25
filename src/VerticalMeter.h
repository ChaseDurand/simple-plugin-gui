#include <juce_gui_basics/juce_gui_basics.h>

class VerticalMeter : public juce::Component{
    public:
        void paint(juce::Graphics& g) override{
            float cornerRound = getWidth() * 0.3f;

            // Meter background
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colour(127, 127, 127));
            g.fillRoundedRectangle(bounds, cornerRound);

            // Meter foreground
            float meterMax = 6.f;
            const auto scaledY = juce::jmap(level, NEGATIVE_INF_THRESH, meterMax, 0.f, (float)getHeight());
            g.setColour(juce::Colours::white);
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), cornerRound);
        }

        void setLevel(const float value) {level = value; }
    private:
        float level = -60.0f;
};
#include <juce_gui_basics/juce_gui_basics.h>

class VerticalMeter : public juce::Component{
    public:
        void paint(juce::Graphics& g) override{
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colour(127, 127, 127));
            g.fillRoundedRectangle(bounds, 5.f);

            g.setColour(juce::Colours::white);

            const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f , (float)getHeight());
            g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 5.f);
        }

        void setLevel(const float value) {level = value; }
    private:
        float level = -60.0f;
};
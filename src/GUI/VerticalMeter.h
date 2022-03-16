#include <juce_gui_basics/juce_gui_basics.h>
#include "../Config.h"
// #define config::NEGATIVE_INF_THRESH -60.0f

class VerticalMeter : public juce::Component
{
    public:
        void paint(juce::Graphics& g) override;
        void setLevel(const float value);
    private:
        float level = config::NEGATIVE_INF_THRESH;
};
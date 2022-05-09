#include <juce_gui_extra/juce_gui_extra.h>
#include "CustomLookAndFeel.h"

class ChannelButton : public juce::TextButton{
    public:
        ChannelButton(const juce::String& buttonName);
        ~ChannelButton() override;
        // void paint(juce::Graphics& g) override;

    private:
        ChannelButtonLookAndFeel channelButtonLookAndFeel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelButton)
};
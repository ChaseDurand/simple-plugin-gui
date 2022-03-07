#include <juce_gui_extra/juce_gui_extra.h>
#include "CustomLookAndFeel.h"

class ChannelButton : public juce::TextButton{
    public:
        ChannelButton(const juce::String& buttonName);
        ~ChannelButton() override;
        // void paint(juce::Graphics& g) override;
        // enum ColourIds
        // {
        //     textColourId            = 0x1006501,  /**< The colour to use for the button's text. */
        //     tickColourId            = 0x1006502,  /**< The colour to use for the tick mark. */
        //     tickDisabledColourId    = 0x1006503,   /**< The colour to use for the disabled tick mark and/or outline. */
        //     borderColourId = 1
        // };
    private:
        ChannelButtonLookAndFeel channelButtonLookAndFeel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelButton)
};
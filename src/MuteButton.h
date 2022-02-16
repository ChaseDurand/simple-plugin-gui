#include <juce_gui_extra/juce_gui_extra.h>
#include "CustomLookAndFeel.h"

class MuteButton : public juce::ToggleButton{
    public:
        MuteButton();
        ~MuteButton() override;
        bool hitTest(int x, int y) override;
    private:
        MuteButtonLookAndFeel muteButtonLookAndFeel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteButton)

};
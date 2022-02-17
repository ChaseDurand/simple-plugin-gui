#include "MuteButton.h"

MuteButton::MuteButton() : juce::ToggleButton(){
    setLookAndFeel(&muteButtonLookAndFeel);
}

MuteButton:: ~MuteButton() {
    setLookAndFeel(nullptr);
}

bool MuteButton::hitTest(int x, int y)
{
    float circum = getWidth();
    float radius = circum / 2.0f;

    float offsetX = (getWidth() - circum) * 0.5f;
    float offsetY = (getHeight() - circum) * 0.5f;

    float centerX = radius;
    float centerY = radius;
        
    juce::Point<float> centerPoint {centerX+offsetX, centerY+offsetY};
    
    return (centerPoint.getDistanceFrom
        (juce::Point<float>( (float)x, (float)y )) <= radius);
}
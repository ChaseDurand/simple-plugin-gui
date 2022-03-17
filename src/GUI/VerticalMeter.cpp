#include "VerticalMeter.h"
#include "CustomColours.h"

// TODO add orange and red for clipping

void VerticalMeter::paint(juce::Graphics& g)
{
    // Meter background
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colour(127, 127, 127));
    g.fillRect(bounds);

    // Meter foreground
    float meterMax = 12.f;
    float scaledY = juce::jmap(level, config::NEGATIVE_INF_THRESH, meterMax, 0.f, static_cast<float>(getHeight()));
    g.setColour(CustomColours::offWhite);
    g.fillRect(bounds.removeFromBottom(scaledY));
}

void VerticalMeter::setLevel(const float value)
{
    level = value;
    return;
}
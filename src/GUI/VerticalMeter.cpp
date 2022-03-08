#include "VerticalMeter.h"

// TODO add orange and red for clipping

void VerticalMeter::paint(juce::Graphics& g)
{
    float cornerRound = getWidth() * 0.3f;

    // Meter background
    auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colour(127, 127, 127));
    g.fillRoundedRectangle(bounds, cornerRound);

    // Meter foreground
    float meterMax = 6.f;
    const auto scaledY = juce::jmap(level, NEGATIVE_INF_THRESH, meterMax, 0.f, static_cast<float>(getHeight()));
    g.setColour(juce::Colours::white);
    g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), cornerRound);
    return;
}

void VerticalMeter::setLevel(const float value)
{
    level = value;
    return;
}
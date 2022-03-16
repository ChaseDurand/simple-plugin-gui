#pragma once
#include <juce_core/juce_core.h>

namespace config
{
    const float NEGATIVE_INF_THRESH = -60.0f;
    const juce::String GAIN_ID("GAIN");
    const juce::String GAIN_NAME("Gain");
    const float GAIN_MAX = 24.0f;
    const juce::String MUTE_ID("MUTE");
    const juce::String MUTE_NAME("Mute");
    const int RADIO_ID_CHANNEL = 01;
    const juce::String CHANNEL_ID("CHANNEL");
    const juce::String CHANNEL_NAME("Channel");
}
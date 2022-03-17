// Constants used throughout the plugin.

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
    const double SMOOTHING_TIME_GAIN = 0.03; // Gain change smoothing time in seconds
    const double SMOOTHING_TIME_METER = 0.5; // Smoothing time for decreasing RMS values
    const int PLUGIN_X_INITIAL = 520;
    const int PLUGIN_X_MIN = 390;
    const int PLUGIN_X_MAX = 3000;
    const float PLUGIN_ASPECT_RATIO = 1.5;
}
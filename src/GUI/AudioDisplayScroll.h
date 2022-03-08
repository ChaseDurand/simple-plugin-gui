#include <juce_audio_utils/juce_audio_utils.h>
#pragma once

class AudioDisplayScroll : public juce::AudioVisualiserComponent,
                           public juce::AudioIODeviceCallback
{
    public:
        AudioDisplayScroll();

        void audioDeviceAboutToStart (juce::AudioIODevice* device) override;
        void audioDeviceStopped() override;

        void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                            float** outputChannelData, int numOutputChannels,
                            int numberOfSamples) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDisplayScroll)
};
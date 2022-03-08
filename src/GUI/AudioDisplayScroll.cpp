#include "AudioDisplayScroll.h"

// TODO reduce to single channel and instantiate two objects with different colors

AudioDisplayScroll::AudioDisplayScroll() : AudioVisualiserComponent(2)
{
    setSamplesPerBlock (256); // Updated in prepareToPlay.
    setBufferSize (350); // Number of blocks saved. Controls "scroll speed".
    return;
}

void AudioDisplayScroll::audioDeviceAboutToStart(juce::AudioIODevice* /*device*/)
{
    clear();
}

void AudioDisplayScroll::audioDeviceStopped()
{
    clear();
}

void AudioDisplayScroll::audioDeviceIOCallback(const float** inputChannelData, int numInputChannels,
                            float** outputChannelData, int numOutputChannels,
                            int numberOfSamples)
{
    for(int i=0; i<numberOfSamples; ++i)
    {
        float inputSample = 0;

        for(int chan=0; chan < numInputChannels; ++chan)
        {
            if(const float* inputChannel = inputChannelData[chan])
            {
                inputSample+=inputChannel[i];
            }
        }

        inputSample *= 10.0f;
        pushSample(&inputSample, 1);
    }

    for(int j=0; j<numOutputChannels; ++j)
    {
        if(float* outputChannel = outputChannelData[j])
        {
            juce::zeromem(outputChannel, (size_t) numberOfSamples * sizeof(float));
        }
    }
}
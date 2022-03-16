#include "AudioVisualiserOverlayed.h"

namespace juce
{

struct AudioVisualiserOverlayed::ChannelInfo
{
    ChannelInfo (AudioVisualiserOverlayed& o, int bufferSize) : owner (o)
    {
        setBufferSize (bufferSize);
        clear();
    }

    void clear() noexcept
    {
        levels.fill ({});
        value = {};
        subSample = 0;
    }

    void pushSamples (const float* inputSamples, int num) noexcept
    {
        for (int i = 0; i < num; ++i)
            pushSample (inputSamples[i]);
    }

    void pushSample (float newSample) noexcept
    {
        if (--subSample <= 0)
        {
            if (++nextSample == levels.size())
                nextSample = 0;

            levels.getReference (nextSample) = value;
            subSample = owner.getSamplesPerBlock();
            value = Range<float> (newSample, newSample);
        }
        else
        {
            value = value.getUnionWith (newSample);
        }
    }

    void setBufferSize (int newSize)
    {
        levels.removeRange (newSize, levels.size());
        levels.insertMultiple (-1, {}, newSize - levels.size());

        if (nextSample >= newSize)
            nextSample = 0;
    }

    AudioVisualiserOverlayed& owner;
    Array<Range<float>> levels;
    Range<float> value;
    std::atomic<int> nextSample { 0 }, subSample { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelInfo)
};

//==============================================================================
AudioVisualiserOverlayed::AudioVisualiserOverlayed (int initialNumChannels)
  : numSamples (1024),
    inputSamplesPerBlock (256),
    backgroundColour (Colours::black),
    waveformColour (Colours::white)
{
    setOpaque (false);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
    setSamplesPerBlock (512);
    setBufferSize (64);
}

AudioVisualiserOverlayed::~AudioVisualiserOverlayed()
{
}

void AudioVisualiserOverlayed::setNumChannels (int numChannels)
{
    channels.clear();

    for (int i = 0; i < numChannels; ++i)
        channels.add (new ChannelInfo (*this, numSamples));
}

void AudioVisualiserOverlayed::setBufferSize (int newNumSamples)
{
    numSamples = newNumSamples;

    for (auto* c : channels)
        c->setBufferSize (newNumSamples);
}

void AudioVisualiserOverlayed::clear()
{
    for (auto* c : channels)
        c->clear();
}

void AudioVisualiserOverlayed::pushBuffer (const float** d, int numChannels, int num)
{
    numChannels = jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (d[i], num);
}

void AudioVisualiserOverlayed::pushBuffer (const AudioBuffer<float>& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void AudioVisualiserOverlayed::pushBuffer (const AudioSourceChannelInfo& buffer)
{
    auto numChannels = jmin (buffer.buffer->getNumChannels(), channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (buffer.buffer->getReadPointer (i, buffer.startSample),
                                               buffer.numSamples);
}

void AudioVisualiserOverlayed::pushSample (const float* d, int numChannels)
{
    numChannels = jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSample (d[i]);
}

void AudioVisualiserOverlayed::setSamplesPerBlock (int newSamplesPerPixel) noexcept
{
    inputSamplesPerBlock = newSamplesPerPixel;
}

void AudioVisualiserOverlayed::setRepaintRate (int frequencyInHz)
{
    startTimerHz (frequencyInHz);
}

void AudioVisualiserOverlayed::timerCallback()
{
    repaint();
}

void AudioVisualiserOverlayed::setColours (Colour bk, Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    repaint();
}

void AudioVisualiserOverlayed::paint (Graphics& g)
{
    g.fillAll (backgroundColour);

    auto r = getLocalBounds().toFloat();

    g.setColour (waveformColour);
    g.setColour(juce::Colour::fromFloatRGBA(0.0f, 0.33f, 1.0f, 0.65f));

    auto* c = channels[0];
        paintChannel (g, r,
            c->levels.begin(), c->levels.size(), c->nextSample);

    g.setColour(juce::Colour::fromFloatRGBA(1.0f, 0.1f, 0.17f, 0.65f));
    c = channels[1];
        paintChannel (g, r,
            c->levels.begin(), c->levels.size(), c->nextSample);
}

void AudioVisualiserOverlayed::audioDeviceIOCallback(const float** inputChannelData,
    int numInputChannels, float** outputChannelData, int numOutputChannels,
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

void AudioVisualiserOverlayed::getChannelAsPath (Path& path, const Range<float>* levels,
                                                 int numLevels, int nextSample)
{
    path.preallocateSpace (4 * numLevels + 8);

    for (int i = 0; i < numLevels; ++i)
    {
        auto level = -(levels[(nextSample + i) % numLevels].getEnd());

        if (i == 0)
            path.startNewSubPath (0.0f, level);
        else
            path.lineTo ((float) i, level);
    }

    for (int i = numLevels; --i >= 0;)
        path.lineTo ((float) i, -(levels[(nextSample + i) % numLevels].getStart()));

    path.closeSubPath();
}

void AudioVisualiserOverlayed::paintChannel (Graphics& g, Rectangle<float> area,
                                             const Range<float>* levels, int numLevels, int nextSample)
{
    Path p;
    getChannelAsPath (p, levels, numLevels, nextSample);
    juce::PathStrokeType stroke(getWidth() * 0.005f);
    
    g.strokePath (p, stroke, AffineTransform::fromTargetPoints (0.0f, -1.0f,               area.getX(), area.getY(),
                                                      0.0f, 1.0f,                area.getX(), area.getBottom(),
                                                      (float) numLevels, -1.0f,  area.getRight(), area.getY()));
}

} // namespace juce
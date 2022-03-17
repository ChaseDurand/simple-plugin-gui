#include "AudioVisualiserOverlayed.h"
#include "CustomColours.h"

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
            value = juce::Range<float> (newSample, newSample);
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
    juce::Array<juce::Range<float>> levels;
    juce::Range<float> value;
    std::atomic<int> nextSample { 0 }, subSample { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelInfo)
};

//==============================================================================
AudioVisualiserOverlayed::AudioVisualiserOverlayed (int initialNumChannels)
  : numSamples (1024),
    inputSamplesPerBlock (256),
    backgroundColour (juce::Colours::black),
    waveformColour (juce::Colours::white)
{
    setOpaque (false);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
    setSamplesPerBlock (512);
    setBufferSize (128);
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
    numChannels = juce::jmin (numChannels, channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (d[i], num);
}

void AudioVisualiserOverlayed::pushBuffer (const juce::AudioBuffer<float>& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void AudioVisualiserOverlayed::pushBuffer (const juce::AudioSourceChannelInfo& buffer)
{
    auto numChannels = juce::jmin (buffer.buffer->getNumChannels(), channels.size());

    for (int i = 0; i < numChannels; ++i)
        channels.getUnchecked(i)->pushSamples (buffer.buffer->getReadPointer (i, buffer.startSample),
                                               buffer.numSamples);
}

void AudioVisualiserOverlayed::pushSample (const float* d, int numChannels)
{
    numChannels = juce::jmin (numChannels, channels.size());

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

void AudioVisualiserOverlayed::setColours (juce::Colour bk, juce::Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    repaint();
}

void AudioVisualiserOverlayed::paint (juce::Graphics& g)
{
    g.fillAll (backgroundColour);

    auto r = getLocalBounds().toFloat();

    // Left channel
    juce::Colour col = CustomColours::blue.withAlpha(0.9f);
    auto* c = channels[0];
        paintChannel (g, r,
            c->levels.begin(), c->levels.size(), c->nextSample, col);

    // Right channel
    col = CustomColours::orange.withAlpha(0.9f);
    c = channels[1];
        paintChannel (g, r,
            c->levels.begin(), c->levels.size(), c->nextSample, col);
}

void AudioVisualiserOverlayed::getChannelAsPath (juce::Path& path, const juce::Range<float>* levels,
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

void AudioVisualiserOverlayed::paintChannel (juce::Graphics& g, juce::Rectangle<float> area,
    const juce::Range<float>* levels, int numLevels, int nextSample, juce::Colour& col)
{
    juce::Path p;
    getChannelAsPath (p, levels, numLevels, nextSample);

    // Outline
    juce::PathStrokeType stroke(getWidth() * 0.008f);
    g.setColour(col);
    g.strokePath (p, stroke, juce::AffineTransform::fromTargetPoints (0.0f, -1.0f, area.getX(), area.getY(),
        0.0f, 1.0f, area.getX(), area.getBottom(),
        (float) numLevels, -1.0f, area.getRight(), area.getY()));

    
    // Fill with increased transparency
    g.setColour(col.withMultipliedAlpha(0.83f));
    g.fillPath (p, juce::AffineTransform::fromTargetPoints (0.0f, -1.0f, area.getX(), area.getY(),
        0.0f, 1.0f, area.getX(), area.getBottom(),
        (float) numLevels, -1.0f, area.getRight(), area.getY()));
}
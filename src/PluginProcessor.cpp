#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessor::SimplePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

SimplePluginAudioProcessor::~SimplePluginAudioProcessor()
{
}

//==============================================================================
const juce::String SimplePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimplePluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SimplePluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SimplePluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SimplePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimplePluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int SimplePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimplePluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SimplePluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SimplePluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void SimplePluginAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    rmsLeft.reset(sampleRate, meterSmoothingLengthSeconds);
    rmsRight.reset(sampleRate, meterSmoothingLengthSeconds);
    rmsLeft.setCurrentAndTargetValue(config::NEGATIVE_INF_THRESH);
    rmsRight.setCurrentAndTargetValue(config::NEGATIVE_INF_THRESH);
    levelSmoothed.reset(sampleRate, gainSmoothingLengthSeconds);
}

void SimplePluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimplePluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void SimplePluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                              juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto muteStatus = apvts.getRawParameterValue(config::MUTE_ID)->load();
    if (muteStatus == 0.0f){
        // Not muted, use gain value.
        auto currentGain = apvts.getRawParameterValue(config::GAIN_ID)->load();
        levelSmoothed.setTargetValue(currentGain);
    }
    else{
        // Muted, use negative infinity gain.
        levelSmoothed.setTargetValue(config::NEGATIVE_INF_THRESH);
    }

    auto channelSelection = apvts.getRawParameterValue(config::CHANNEL_ID)->load();

    // Because we are applying a gain ramp across the buffer,
    // process all channels per sample (vs all samples per channels).
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        auto gainToApply = juce::Decibels::decibelsToGain(levelSmoothed.getNextValue(), config::NEGATIVE_INF_THRESH);
        if(channelSelection == 1)
        {
            // Stereo
            for(int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                auto *channelData = buffer.getWritePointer(channel);
                channelData[sample] *= gainToApply;
            }
        }
        else
        {
            // Single channel
            // Default left
            int sourceChannel = 0;
            if(channelSelection == 2)
            {
                // Right
                sourceChannel = 1;
            }

            // Only process source channel and copy result to all channels
            auto *channelDataSource = buffer.getWritePointer(sourceChannel);
            channelDataSource[sample] *= gainToApply;
            for(int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                if(channel != sourceChannel)
                {
                    auto *channelDataOut = buffer.getWritePointer(channel);
                    channelDataOut[sample] = channelDataSource[sample];
                }
            }
        }


    }

    // Update scrolling waveform
    audioDisplayScroll.pushBuffer(buffer);

    // Increment rms smoothed value.
    rmsLeft.skip(buffer.getNumSamples());
    rmsRight.skip(buffer.getNumSamples());

    // Calculate values for RMS meter.
    // Immediately move meter when values increase smooth with decrease.
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (value < rmsLeft.getCurrentValue())
        {
            rmsLeft.setTargetValue(value);
        }
        else
        {
            rmsLeft.setCurrentAndTargetValue(value);
        }
    }
    {
        const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (value < rmsRight.getCurrentValue())
        {
            rmsRight.setTargetValue(value);
        }
        else
        {
            rmsRight.setCurrentAndTargetValue(value);
        }
    }
}

//==============================================================================
bool SimplePluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SimplePluginAudioProcessor::createEditor()
{
    return new SimplePluginAudioProcessorEditor(*this);
}

//==============================================================================
void SimplePluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto editor = apvts.state.getOrCreateChildWithName("editor", nullptr);
    editor.setProperty("size-x", editorSize.x, nullptr);
    editor.setProperty("size-y", editorSize.y, nullptr);
    juce::MemoryOutputStream(destData, false);
    juce::ignoreUnused(destData);
}

void SimplePluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    if(tree.isValid())
    {
        apvts.state = tree;
        auto editor = apvts.state.getChildWithName("editor");
        if(editor.isValid())
        {
            editorSize.setX(editor.getProperty("size-x", 520));
            editorSize.setY(editor.getProperty("size-y", 520 / 2.0));
            if(auto* thisEditor = getActiveEditor())
            {
                thisEditor->setSize(editorSize.x, editorSize.y);
            }
        }
    }
    juce::ignoreUnused(data, sizeInBytes);
}

juce::Point<int> SimplePluginAudioProcessor::getSavedSize() const
{
    return editorSize;
}

void SimplePluginAudioProcessor::setSavedSize(const juce::Point<int>& size)
{
    editorSize = size;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new SimplePluginAudioProcessor();
}

float SimplePluginAudioProcessor::getRmsValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return rmsLeft.getCurrentValue();
    if (channel == 1)
        return rmsRight.getCurrentValue();
    return 0.f;
}


juce::AudioProcessorValueTreeState::ParameterLayout SimplePluginAudioProcessor::createParameters(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterBool>
        (config::MUTE_ID, config::MUTE_NAME, false));
    
    // Calculate skew value for gain knob to set 0dB at 12 o'clock.
    float gainCenterPoint = 0.0f;
    float gainSkew = std::log (static_cast<float> (0.5)) /
        std::log ((gainCenterPoint - config::NEGATIVE_INF_THRESH)
            / (config::GAIN_MAX - config::NEGATIVE_INF_THRESH));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
    (config::GAIN_ID,
    config::GAIN_NAME,
    juce::NormalisableRange<float>(config::NEGATIVE_INF_THRESH, config::GAIN_MAX, 0, gainSkew, false),
    0.0f,
    juce::String(),
    juce::AudioProcessorParameter::genericParameter,
    // Return "-inf" to host if gain is at lower threshold
    [](float value, int maximumStringLength) {
        if ((value > config::NEGATIVE_INF_THRESH) || (maximumStringLength < 4)){
            return juce::Decibels::toString(value);
        }
        else{
            return juce::String("-inf");
        };
    },
    nullptr
    ));

    parameters.push_back(std::make_unique<juce::AudioParameterChoice>
        (config::CHANNEL_ID,
		config::CHANNEL_NAME,
		juce::StringArray {"Left", "Stereo", "Right"},
		1));

    return { parameters.begin(), parameters.end() };
}
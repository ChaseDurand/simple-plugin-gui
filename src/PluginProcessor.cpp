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
void SimplePluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    auto muteStatus = apvts.getRawParameterValue(MUTE_ID)->load();
    if (muteStatus == 0.0f){
        // Not muted, use gain value.
        auto currentGain = apvts.getRawParameterValue(GAIN_ID)->load();
        levelSmoothed.setTargetValue(currentGain);
    }
    else{
        // Muted, use negative infinity gain.
        levelSmoothed.setTargetValue(NEGATIVE_INF_THRESH);
    }

    rmsLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    rmsRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));



    // Because we are applying a gain ramp across the buffer,
    // process all channels per sample (vs all samples per channels).
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        auto gainToApply = juce::Decibels::decibelsToGain((float)levelSmoothed.getNextValue(), NEGATIVE_INF_THRESH);
        for (int channel = 0; channel < totalNumInputChannels; ++channel){
            auto *channelData = buffer.getWritePointer(channel);
            channelData[sample] *= gainToApply;
            juce::ignoreUnused(channelData);
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void SimplePluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
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
        return rmsLeft;
    if (channel == 1)
        return rmsRight;
    return 0.f;
}


juce::AudioProcessorValueTreeState::ParameterLayout SimplePluginAudioProcessor::createParameters(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterBool>
        (MUTE_ID, MUTE_NAME, false));
    
    // Calculate skew value for gain knob to set 0dB at 12 o'clock.
    float gainCenterPoint = 0.0f;
    float gainSkew = std::log (static_cast<float> (0.5)) /
        std::log ((gainCenterPoint - NEGATIVE_INF_THRESH)
            / (GAIN_MAX - NEGATIVE_INF_THRESH));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>
    (GAIN_ID,
    GAIN_NAME,
    juce::NormalisableRange<float>(NEGATIVE_INF_THRESH, GAIN_MAX, 0, gainSkew, false),
    0.0f,
    juce::String(),
    juce::AudioProcessorParameter::genericParameter,
    // Return "-inf" to host if gain is at lower threshold
    [](float value, int maximumStringLength) {
        if ((value > NEGATIVE_INF_THRESH) || (maximumStringLength < 4)){
            return juce::Decibels::toString(value);
        }
        else{
            return juce::String("-inf");
        };
    },
    nullptr
    ));



    parameters.push_back(std::make_unique<juce::AudioParameterChoice>
        (CHANNEL_ID,
		CHANNEL_NAME,
		juce::StringArray {"Left", "Center", "Right"},
		1));

    return { parameters.begin(), parameters.end() };
}
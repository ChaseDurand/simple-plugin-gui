#pragma once
#include "Config.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include "GUI/AudioVisualiserOverlayed.h"


//==============================================================================
class SimplePluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimplePluginAudioProcessor();
    ~SimplePluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    juce::Point<int> getSavedSize() const;
    void setSavedSize(const juce::Point<int>& size);
    
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> levelSmoothed = 1.0;
    double gainSmoothingLengthSeconds = 0.03;
    double meterSmoothingLengthSeconds = 0.5;

    float getRmsValue(const int channel) const;

    juce::AudioVisualiserOverlayed audioDisplayScroll {2};
    juce::AudioDeviceManager audioDeviceManager;

private:
    //==============================================================================

    juce::LinearSmoothedValue<float> rmsLeft, rmsRight;

    juce::Point<int> editorSize = {522, 348};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimplePluginAudioProcessor)
};

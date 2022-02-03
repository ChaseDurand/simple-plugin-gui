#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    sliderValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, GAIN_ID, decibelSlider);

    decibelLabel.setText ("Gain", juce::dontSendNotification);
    addAndMakeVisible(decibelLabel);

    decibelSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    decibelSlider.setValue (juce::Decibels::gainToDecibels (level, -60.0f));
    decibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain ((float) decibelSlider.getValue(), -60.0f); };
    decibelSlider.setRange(-60.0f, 12.0f, 0.01f);
    decibelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    decibelSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(decibelSlider);

    mGainSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    mGainSlider.setRange(-60.0f, 12.0f, 0.01f);
    mGainSlider.setValue(0.0f); // TODO fix negative zero
    mGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mGainSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(mGainSlider);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(700, 300);
}

SimplePluginAudioProcessorEditor::~SimplePluginAudioProcessorEditor()
{
}

//==============================================================================
void SimplePluginAudioProcessorEditor::paint(juce::Graphics &g)
{

    g.fillAll (juce::Colours::black);
    
}

void SimplePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    mGainSlider.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 75, 100, 150);
    decibelSlider.setBounds(getWidth() / 4, getHeight() / 2 - 75, 100, 150);
    decibelLabel.setBounds(getWidth() / 4, 10, 60, 20);
}

void SimplePluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){
    if (slider == &mGainSlider){
        processorRef.mGain = (float)mGainSlider.getValue();
    }
}


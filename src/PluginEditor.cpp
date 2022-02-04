#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    sliderValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.treeState, GAIN_ID, rotaryDecibelSlider);

    rotaryDecibelLabel.setText ("Gain", juce::dontSendNotification);
    addAndMakeVisible(rotaryDecibelLabel);

    rotaryDecibelSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    rotaryDecibelSlider.setSkewFactorFromMidPoint(0.0f);
    rotaryDecibelSlider.setValue (juce::Decibels::gainToDecibels (level, NEGATIVE_INF_THRESH));
    rotaryDecibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain ((float) rotaryDecibelSlider.getValue(), NEGATIVE_INF_THRESH); };
    rotaryDecibelSlider.setRange(NEGATIVE_INF_THRESH, 24.0f, 0.1f);
    rotaryDecibelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    rotaryDecibelSlider.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(rotaryDecibelSlider);

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
    g.setGradientFill(juce::ColourGradient(juce::Colour(50, 51, 55), 0, 0,
                                            juce::Colour(22, 22, 22), getWidth(), getHeight(), false));
    g.fillAll();
    
}

void SimplePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    rotaryDecibelSlider.setBounds(getWidth() / 2, getHeight() / 2 - 75, 80, 80);
    rotaryDecibelLabel.setBounds(getWidth() / 2, 10, 60, 20);
}

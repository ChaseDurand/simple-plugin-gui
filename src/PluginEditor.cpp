#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);
    gainKnob.setSliderStyle
        (juce::Slider::SliderStyle::RotaryVerticalDrag);
    gainKnob.setRange(NEGATIVE_INF_THRESH, GAIN_MAX, 0.1f);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow,
                                                        true, 80, 30);
    gainKnob.setDoubleClickReturnValue(true, 0.0f);
    addAndMakeVisible(gainKnob);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, GAIN_ID, gainKnob);

    addAndMakeVisible(muteButton);
    muteButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, MUTE_ID, muteButton);

    buttonChannelL.setRadioGroupId(RADIO_ID_CHANNEL);
    buttonChannelC.setRadioGroupId(RADIO_ID_CHANNEL);
    buttonChannelR.setRadioGroupId(RADIO_ID_CHANNEL);

    // buttonChannelL.onClick() = [this](){ 
    //     this.setToggleState(true, juce::NotificationType::sendNotification);
    // };

    // channelButtonLAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
    //     processorRef.apvts, CHANNEL_ID, buttonChannelL);

    // channelButtonCAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
    //     processorRef.apvts, CHANNEL_ID, buttonChannelC);

    buttonChannelC.setToggleState(true, juce::NotificationType::sendNotification);

    addAndMakeVisible(buttonChannelL);
    addAndMakeVisible(buttonChannelC);
    addAndMakeVisible(buttonChannelR);

    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
    startTimerHz(24);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(520, 260);
}

SimplePluginAudioProcessorEditor::~SimplePluginAudioProcessorEditor()
{
}

//==============================================================================
void SimplePluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.setGradientFill(juce::ColourGradient(juce::Colour(50, 50, 50), 0, 0,
                                           juce::Colour(20, 20, 20), getWidth(),
                                           getHeight(), false));
    g.fillAll();
}

void SimplePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    muteButton.setBounds(getWidth() * 0.04f, getHeight() / 2 - 50, 80, 80);
    gainKnob.setBounds(getWidth() * 0.22f, getHeight() / 2 - 60, 100, 120);
    buttonChannelL.setBounds(getWidth() * 0.5f, getHeight() / 2 - 60, 100, 120);
    buttonChannelC.setBounds(getWidth() * 0.6f, getHeight() / 2 - 60, 100, 120);
    buttonChannelR.setBounds(getWidth() * 0.7f, getHeight() / 2 - 60, 100, 120);
    meterL.setBounds(2, 2, 200, 15);
    meterR.setBounds(2, 20, 200, 15);
}

void SimplePluginAudioProcessorEditor::timerCallback()
{
    meterL.setLevel(processorRef.getRmsValue(0));
    meterR.setLevel(processorRef.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
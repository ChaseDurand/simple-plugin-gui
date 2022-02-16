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
    g.setGradientFill(juce::ColourGradient(juce::Colour(60, 60, 60), 0, 0,
                                           juce::Colour(30, 30, 30), getWidth(),
                                           getHeight(), false));
    g.fillAll();
}

void SimplePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    muteButton.setBounds(getWidth() / 4 - 50, getHeight() / 2 - 50, 100, 100);
    gainKnob.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 60, 100, 120);

}

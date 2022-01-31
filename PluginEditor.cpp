#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    mGainSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    mGainSlider.setRange(-60.0f, 12.0f, 0.01f);
    mGainSlider.setValue(0.0f);
    mGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    addAndMakeVisible(mGainSlider);
    mGainSlider.addListener(this);

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
}

void SimplePluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider){
    if (slider == &mGainSlider){
        processorRef.mGain = (float)mGainSlider.getValue();
    }
}
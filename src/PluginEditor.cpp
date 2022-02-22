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

    channelButtonAttachment = std::make_unique<juce::ParameterAttachment>(*processorRef.apvts.getParameter(CHANNEL_ID),
        [this](float value)
    {
            int index = std::floor (value / channelButtons.size());
            if (juce::isPositiveAndBelow (index, channelButtons.size())){
                channelButtons.at (index) -> setToggleState (true, juce::NotificationType::dontSendNotification);
            }   
    }, nullptr);
    
    auto makeButton = [this](auto name, int index)
    {
        auto button = std::make_unique<juce::TextButton>(name);
        button->setClickingTogglesState (true);
        button->setRadioGroupId (RADIO_ID_CHANNEL);
        addAndMakeVisible (button.get());
        button->onStateChange = [&, index]
        {
            if (button->getToggleState())
            {
                channelButtonAttachment->setValueAsCompleteGesture(index);
            }
        };
        return button;
    };

    channelButtons.push_back(makeButton ("Foo", 0));
    channelButtons.push_back(makeButton ("Bar", 1));
    channelButtons.push_back(makeButton ("Baz", 2));

    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
    startTimerHz(24);
    
    double ratio = 2.0;
    setResizable(true, true);
    setResizeLimits(390, 390 / ratio, 1560, 1560 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(520, 520 / ratio);
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
    meterL.setBounds(getWidth() * 0.9f, 20, 15, 200);
    meterR.setBounds(getWidth() * 0.9f + 20, 20, 15, 200);


    // if (channelButtons.size() == 3){
    //     channelButtons[0].get()->setBounds(getWidth() * 0.5f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[1].get()->setBounds(getWidth() * 0.6f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[2].get()->setBounds(getWidth() * 0.7f, getHeight() / 2 - 60, 60, 30);
    // }
}

void SimplePluginAudioProcessorEditor::timerCallback()
{
    meterL.setLevel(processorRef.getRmsValue(0));
    meterR.setLevel(processorRef.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
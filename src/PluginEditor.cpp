#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    gainKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0, 0, 0));
    gainKnob.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(72, 72, 72));
    gainKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(110, 221, 202));


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
    
    juce::Point<int> size = processorRef.getSavedSize();
    double ratio = 2.0;
    setResizable(true, true);
    setResizeLimits(390, 390 / ratio, 3000, 3000 / ratio);
    getConstrainer()->setFixedAspectRatio(ratio);
    setSize(size.x, size.y);

    setWantsKeyboardFocus(true);
}

SimplePluginAudioProcessorEditor::~SimplePluginAudioProcessorEditor()
{
}

//==============================================================================
void SimplePluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour(35, 35, 35));
}

void SimplePluginAudioProcessorEditor::resized()
{
    processorRef.setSavedSize({getWidth(), getHeight()});

    float widthUnit = getWidth() * 0.2;

    muteButton.setBounds(0, (getHeight() - widthUnit * 0.9f) * 0.5f, widthUnit * 0.9f, widthUnit * 0.9f);
    gainKnob.setBounds(widthUnit * 1, (getHeight() - widthUnit * 0.9f) * 0.5f, widthUnit * 0.9f, widthUnit * 0.9f);

    // if (channelButtons.size() == 3){
    //     channelButtons[0].get()->setBounds(getWidth() * 0.5f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[1].get()->setBounds(getWidth() * 0.6f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[2].get()->setBounds(getWidth() * 0.7f, getHeight() / 2 - 60, 60, 30);
    // }

    float meterHeightPercent = 0.8;
    float meterHeight = getHeight() * meterHeightPercent;
    float meterMargin = getHeight() * (1 - meterHeightPercent) * 0.5;
    meterL.setBounds(widthUnit * 4.2, meterMargin, widthUnit * 0.26, meterHeight);
    meterR.setBounds(widthUnit * 4.5, meterMargin, widthUnit * 0.26, meterHeight);
}

void SimplePluginAudioProcessorEditor::timerCallback()
{
    meterL.setLevel(processorRef.getRmsValue(0));
    meterR.setLevel(processorRef.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
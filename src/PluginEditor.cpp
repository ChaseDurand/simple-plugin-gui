#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/CustomColours.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    // Mute Button
    muteButton.setColour(MuteButton::ColourIds::borderColourId, CustomColours::offWhite);
    muteButton.setColour(MuteButton::ColourIds::tickDisabledColourId, CustomColours::grey);
    muteButton.setColour(MuteButton::ColourIds::tickColourId, CustomColours::red);
    
    addAndMakeVisible(muteButton);
    muteButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, MUTE_ID, muteButton);

    // Gain Knob
    gainKnob.setColour(juce::Slider::backgroundColourId , CustomColours::offWhite);
    gainKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    gainKnob.setColour(juce::Slider::rotarySliderFillColourId, CustomColours::blue);
    gainKnob.setColour(juce::Slider::rotarySliderOutlineColourId, CustomColours::grey);
    addAndMakeVisible(gainKnob);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, GAIN_ID, gainKnob);

    // Channel Selector
    channelButtonAttachment = std::make_unique<juce::ParameterAttachment>(*processorRef.apvts.getParameter(CHANNEL_ID),
        [this](float value)
    {
            unsigned int index = static_cast<unsigned int>(std::floor(value / channelButtons.size()));
            if (juce::isPositiveAndBelow(index, channelButtons.size())){
                channelButtons[index] -> setToggleState (true, juce::NotificationType::dontSendNotification);
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
    int ratio = 2;
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

    int outerMargin = (getWidth() * 0.03f);
    juce::Rectangle<int> bounds = juce::Rectangle<int>
        (getX(), getY(), getWidth(), getHeight()).reduced(outerMargin);

    float widthUnit = bounds.getWidth() * 0.2f;

    muteButton.setBounds(outerMargin,
                         (getHeight() - widthUnit * 0.9f) * 0.5f,
                         widthUnit * 0.9f,
                         widthUnit * 0.9f);
    gainKnob.setBounds(outerMargin + widthUnit * 1,
                       (getHeight() - widthUnit * 0.9f) * 0.5f,
                       widthUnit * 0.9f,
                       widthUnit * 0.9f);

    // if (channelButtons.size() == 3){
    //     channelButtons[0].get()->setBounds(getWidth() * 0.5f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[1].get()->setBounds(getWidth() * 0.6f, getHeight() / 2 - 60, 60, 30);
    //     channelButtons[2].get()->setBounds(getWidth() * 0.7f, getHeight() / 2 - 60, 60, 30);
    // }

    float meterHeightPercent = 0.8f;
    int meterHeight = getHeight() * meterHeightPercent;
    int meterMargin =getHeight() * (1.0f - meterHeightPercent) * 0.5f;
    meterL.setBounds((outerMargin + widthUnit * 4.2),
                     meterMargin,
                     (widthUnit * 0.26),
                     meterHeight);
    meterR.setBounds((outerMargin + widthUnit * 4.5),
                     meterMargin,
                     (widthUnit * 0.26),
                     meterHeight);
}

void SimplePluginAudioProcessorEditor::timerCallback()
{
    meterL.setLevel(processorRef.getRmsValue(0));
    meterR.setLevel(processorRef.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GUI/CustomColours.h"

//==============================================================================
SimplePluginAudioProcessorEditor::SimplePluginAudioProcessorEditor(SimplePluginAudioProcessor &p)
    : AudioProcessorEditor(&p), processorRef(p)
{

    // Audio Waveform Display
    addAndMakeVisible(processorRef.audioDisplayScroll);

    // Mute Button
    muteButton.setColour(MuteButton::ColourIds::borderColourId, CustomColours::offWhite);
    muteButton.setColour(MuteButton::ColourIds::tickDisabledColourId, CustomColours::grey);
    muteButton.setColour(MuteButton::ColourIds::tickColourId, CustomColours::red);
    
    addAndMakeVisible(muteButton);
    muteButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, config::MUTE_ID, muteButton);

    // Gain Knob
    gainKnob.setColour(juce::Slider::backgroundColourId, CustomColours::offWhite);
    gainKnob.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    gainKnob.setColour(juce::Slider::rotarySliderFillColourId, CustomColours::blue);
    gainKnob.setColour(juce::Slider::rotarySliderOutlineColourId, CustomColours::grey);
    addAndMakeVisible(gainKnob);
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, config::GAIN_ID, gainKnob);

    // Channel Selectors
    channelButtonAttachment = std::make_unique<juce::ParameterAttachment>(
        *processorRef.apvts.getParameter(config::CHANNEL_ID), [this](float value)
    {
        unsigned int index = static_cast<unsigned int>(value);
        if (juce::isPositiveAndBelow(index, channelButtons.size())){
            channelButtons[index] -> setToggleState (true, juce::NotificationType::dontSendNotification);
        }
    }, nullptr);
    
    auto makeCB = [this](juce::String name, int index) ->
        std::unique_ptr<juce::Button>
    {
        std::unique_ptr<juce::Button> button = std::make_unique<ChannelButton>(name);
        button->setClickingTogglesState (true);
        button->setRadioGroupId (config::RADIO_ID_CHANNEL);
        addAndMakeVisible (button.get());
        button->onClick = [&b = *button, &cba = *channelButtonAttachment, index]
        {
            if (b.getToggleState())
            {
                cba.setValueAsCompleteGesture(index);
            }
        };
        return button;
    };

    channelButtons.push_back(makeCB (juce::String("Left"), 0));
    channelButtons.back()->setConnectedEdges(juce::TextButton::ConnectedOnRight);
    channelButtons.push_back(makeCB (juce::String("Stereo"), 1));
    channelButtons.back()->setToggleState(true, juce::NotificationType::dontSendNotification);
    channelButtons.back()->setConnectedEdges(juce::TextButton::ConnectedOnLeft |
                                             juce::TextButton::ConnectedOnRight);
    channelButtons.push_back(makeCB (juce::String("Right"), 2));
    channelButtons.back()->setConnectedEdges(juce::TextButton::ConnectedOnLeft);
    
    // VU Meters
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);
    startTimerHz(24); // Refresh rate for meters
    
    // Size plugin window, saving and restoring size for reopening
    juce::Point<int> size = processorRef.getSavedSize();
    float ratio = 1.5f; // Plugin aspect ratio x/y
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

    int audioDisplayWidth = bounds.getWidth() * 0.8f;
    int audioDisplayHeight = bounds.getHeight() * 0.66f;
    int verticalMargin = bounds.getHeight() * 0.05f;
    int lowerComponentY = outerMargin + audioDisplayHeight + verticalMargin;

    processorRef.audioDisplayScroll.setBounds(bounds.getX(), bounds.getY(), audioDisplayWidth, audioDisplayHeight);

    muteButton.setBounds(outerMargin,
                         lowerComponentY,
                         widthUnit * 0.9f,
                         widthUnit * 0.9f);
    gainKnob.setBounds(outerMargin + widthUnit * 1,
                       lowerComponentY,
                       widthUnit * 0.9f,
                       widthUnit * 0.9f);

    if (channelButtons.size() == 3){
        int buttonWidth = widthUnit * 0.65f;
        int buttonHeight = widthUnit * 0.9f;
        channelButtons[0].get()->setBounds(outerMargin + widthUnit * 2, lowerComponentY, buttonWidth, buttonHeight);
        channelButtons[1].get()->setBounds(outerMargin + widthUnit * 2.66, lowerComponentY, buttonWidth, buttonHeight);
        channelButtons[2].get()->setBounds(outerMargin + widthUnit * 3.32, lowerComponentY, buttonWidth, buttonHeight);
    }

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
    // Refresh levels for meter
    meterL.setLevel(processorRef.getRmsValue(0));
    meterR.setLevel(processorRef.getRmsValue(1));
    meterL.repaint();
    meterR.repaint();
}
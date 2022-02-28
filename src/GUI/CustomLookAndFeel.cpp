#include "CustomLookAndFeel.h"
#include "CustomColours.h"
#include "MuteButton.h"

RotaryDecibelSliderLookAndFeel::RotaryDecibelSliderLookAndFeel() : LookAndFeel_V4()
{
    setColour (juce::Slider::thumbColourId, juce::Colours::black);
}

juce::Slider::SliderLayout RotaryDecibelSliderLookAndFeel::getSliderLayout(juce::Slider& slider)
{
    juce::Rectangle<int> localBounds = slider.getLocalBounds();
    juce::Slider::SliderLayout layout;
    layout.textBoxBounds = localBounds.withY(-1);
    layout.sliderBounds = localBounds;
    return layout;
}


void RotaryDecibelSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, const float rotaryStartAngle, const float
    rotaryEndAngle, juce::Slider& slider)
{

    float outerMargin = width * 0.03f;
    juce::Rectangle<float> bounds = juce::Rectangle<float>
        (x, y, width, height).reduced(outerMargin);
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f;
    float centreX = x + width * 0.5f;
    float centreY = y + width * 0.5f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    juce::Path p;
    float arcWidth = radius * 0.2f;
    float arcRadius = radius + arcWidth * 1.2f;

    juce::Colour dialColour = CustomColours::offWhite;
    juce::Colour arcBgColour = CustomColours::grey;
    juce::Colour arcFgColour = CustomColours::blue;
    

    // Outer circle fill
    g.setColour(dialColour);
    g.fillEllipse (centreX - radius,
                   centreY - radius,
                   radius * 2,
                   radius * 2);

    // Arc
    {
        // Outer arc background
        
        p.clear();
        p.startNewSubPath(centreX + arcRadius * sin(rotaryStartAngle), centreY - arcRadius * cos(rotaryStartAngle));
        p.addCentredArc(centreX, centreY, arcRadius, arcRadius,
            0.0f, rotaryStartAngle, rotaryEndAngle, false);
        g.setColour(arcBgColour);
        g.strokePath(p, juce::PathStrokeType(arcWidth, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

        // Outer arc foreground glow
        p.clear();
        p.startNewSubPath(centreX + arcRadius * sin(rotaryStartAngle), centreY - arcRadius * cos(rotaryStartAngle));
        p.addCentredArc(centreX, centreY, arcRadius, arcRadius,
            0.0f, rotaryStartAngle, angle, false);
        g.setColour(arcFgColour);
        g.strokePath(p, juce::PathStrokeType(arcWidth, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));
    }

    // Pointer
    {  
        float pointerWidth = radius * 0.3f;
        float pointerLength = arcRadius * 1.2f;
        // Pointer border
        p.clear();
        p.addRectangle(-pointerWidth * 0.5f, -pointerLength, pointerWidth, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated (centreX, centreY));
        g.setColour(dialColour);
        g.fillPath(p);
    }



    return;
}

RisizableTextBox* RotaryDecibelSliderLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto* textBox = new RisizableTextBox();
    textBox->setColour ( juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    // textBox->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    return textBox; 
}

///////////////////////////////////////
//            Mute Button            //
///////////////////////////////////////

void MuteButtonLookAndFeel::drawTickBox (juce::Graphics& g, juce::Component& button, float x, float y,
    float w, float h, bool ticked, bool isEnabled,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    float outerMargin = w * 0.03f;
    juce::Rectangle<float> bounds = juce::Rectangle<float>
        (x, y, button.getWidth(), button.getHeight()).reduced(outerMargin);
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f;
    float outerRadius = radius * 1.44f;
    float innerRadius = radius / 1.44f;

    // Outer circle
    g.setColour(button.findColour(MuteButton::ColourIds::borderColourId));
    g.fillEllipse (button.getWidth() * 0.5f - radius,
        button.getHeight() * 0.5f - radius,
        radius * 2.0f, radius * 2.0f);

    if (ticked)
    {
        // Enabled
        g.setColour(button.findColour(MuteButton::ColourIds::tickColourId));
    }
    else{
        // Disabled
        g.setColour(button.findColour(MuteButton::ColourIds::tickDisabledColourId));
    }

    // Inner circle/LED
    g.fillEllipse (button.getWidth() * 0.5f - innerRadius,
        button.getWidth() * 0.5f - innerRadius,
        innerRadius * 2.0f, innerRadius * 2.0f);

    return;
}
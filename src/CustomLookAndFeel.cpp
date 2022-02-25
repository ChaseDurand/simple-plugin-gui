#include "CustomLookAndFeel.h"

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
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) width * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    juce::Path p;
    float arcWidth = radius * 0.2f;
    float arcRadius = radius + arcWidth * 1.2f;

    juce::Colour dialColour = juce::Colours::white;
    juce::Colour arcBgColour = slider.findColour(juce::Slider::textBoxOutlineColourId);
    juce::Colour arcFgColour = slider.findColour(juce::Slider::rotarySliderFillColourId);
    

    // Outer circle fill
    g.setColour(dialColour);
    g.fillEllipse (rx, ry, rw, rw);

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

    textBox->setColour (juce::Label::textColourId,            slider.findColour (juce::Slider::textBoxTextColourId));
    textBox->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));
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
    g.setColour(juce::Colours::white);
    g.fillEllipse (button.getWidth() * 0.5f - radius,
        button.getHeight() * 0.5f - radius,
        radius * 2.0f, radius * 2.0f);

    if (ticked)
    {
        // Enabled
        // Light color
        g.setColour(juce::Colours::red);
    }
    else{
        // Disabled
        // Light color
        g.setColour(juce::Colours::grey);
    }

    // Inner circle/LED
    g.fillEllipse (button.getWidth() * 0.5f - innerRadius,
        button.getWidth() * 0.5f - innerRadius,
        innerRadius * 2.0f, innerRadius * 2.0f);

    return;
}
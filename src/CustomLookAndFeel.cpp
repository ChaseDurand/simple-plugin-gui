// #include <juce_gui_basics/juce_gui_basics.h>
#include "CustomLookAndFeel.h"

RotaryDecibelSliderLookAndFeel::RotaryDecibelSliderLookAndFeel() : LookAndFeel_V4(){
    setColour (juce::Slider::thumbColourId, juce::Colours::black);
}

void RotaryDecibelSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, const float rotaryStartAngle, const float
    rotaryEndAngle, juce::Slider&){
    
    auto radius = (float) (width / 2) * (0.7f);
    auto centreX = (float) x + (float) width * 0.5f;
    auto centreY = (float) y + (float) width * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    float innerRadiusPercent = 0.76f;
    juce::Path p;

    // Drop shadow
    // float shadowWidth = 1.8f * radius;
    // int shadowRadius = (int)(2.0f * radius);
    // auto shadow = juce::DropShadow(juce::Colours::black, shadowRadius, juce::Point<int>(0,0));
    // p.clear();
    // p.startNewSubPath(centreX - 0.5f * shadowWidth, ry + 0.5f * radius);
    // p.addEllipse(centreX - 0.5f * shadowWidth,  ry + 0.5f * radius, shadowWidth, shadowWidth);
    // shadow.drawForPath(g, p);

    // Outer circle gradient
    g.setGradientFill(juce::ColourGradient(juce::Colour(114, 114, 114), centreX, ry,
                                            juce::Colour(32, 32, 32), centreX, centreY + radius, false));
    g.fillEllipse (rx, ry, rw, rw);

    // Outer cirlce outline
    g.setColour (juce::Colours::black);
    g.drawEllipse (rx, ry, rw, rw, 2.0f);

    // Inner circle fill
    g.setColour (juce::Colour(62, 62, 62));
    g.fillEllipse (rx + radius * (1.0f-innerRadiusPercent), ry + radius * (1.0f-innerRadiusPercent), rw * innerRadiusPercent, rw * innerRadiusPercent);

    // Inner ring outline
    g.setColour (juce::Colours::black);
    g.drawEllipse (rx + radius * (1.0f-innerRadiusPercent), ry + radius * (1.0f-innerRadiusPercent), rw * innerRadiusPercent, rw * innerRadiusPercent, 2.0f);

    // Pointer border
    auto pointerLength = radius * 0.8f;
    auto pointerThickness = 9.0f;
    p.clear();
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour(juce::Colours::black);
    g.fillPath(p);

    // Pointer inner color
    p.clear();
    pointerLength = radius * 0.66f;
    pointerThickness *= 0.5f;
    p.addRectangle(-pointerThickness * 0.5f, -radius * 0.93f, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour(juce::Colour(110, 221, 202));
    g.fillPath(p);

    // Outer arc background
    float arcSize = 1.3f * radius;
    p.clear();
    p.startNewSubPath(centreX + arcSize * sin(rotaryStartAngle), centreY - arcSize * cos(rotaryStartAngle));
    p.addCentredArc(centreX, centreY, arcSize, arcSize,
        0.0f, rotaryStartAngle, rotaryEndAngle, false);
    g.setColour(juce::Colour(72, 72, 72));
    g.strokePath(p, juce::PathStrokeType(4.0f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    // Outer arc foreground glow
    p.clear();
    p.startNewSubPath(centreX + arcSize * sin(rotaryStartAngle), centreY - arcSize * cos(rotaryStartAngle));
    p.addCentredArc(centreX, centreY, arcSize, arcSize,
        0.0f, rotaryStartAngle, angle, false);
    g.setColour(juce::Colour(110, 221, 202));
    g.strokePath(p, juce::PathStrokeType(4.0f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));


    return;
}

juce::Label* RotaryDecibelSliderLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto* l = juce::LookAndFeel_V2::createSliderTextBox (slider);
    if (getCurrentColourScheme() == juce::LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
                                                                               || slider.getSliderStyle() == juce::Slider::LinearBarVertical))
    {
        l->setColour (juce::Label::textColourId, juce::Colours::black.withAlpha (0.7f));
    }
    
    l->setColour (juce::Label::outlineColourId, juce::Colours::black.withAlpha(0.0f)); // Make label transparant
    l->setColour (juce::Label::textColourId, juce::Colour(170, 170, 170));
    return l;
}

///////////////////////////////////////
//            Mute Button            //
///////////////////////////////////////

void MuteButtonLookAndFeel::drawTickBox (juce::Graphics& g, juce::Component& button, float x, float y,
    float w, float h, bool ticked, bool isEnabled,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{

    float circum = 94.0f;
    float radius = circum / 2.0f;

    float offsetX = (button.getWidth() - circum) * 0.5f;
    float offsetY = (button.getHeight() - circum) * 0.5f;


    if (ticked)
    {
        // Enabled
        g.setColour(juce::Colour(110, 221, 202));
    }
    else{
        // Disabled
        g.setColour (juce::Colours::black);
    }
    g.drawEllipse (0.0f+offsetX, 0.0f+offsetY, circum, circum, 2.0f);

    return;
}

// void MuteButtonLookAndFeel::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
// {
//     auto h = float(getHeight());
//     auto area = getLocalBounds().toFloat();

//     juce::Path p;  // leaving as a path so an optional outline can be added
//     p.addRoundedRectangle(area, h / 2.0f);
//     g.setColour(Colours::darkgrey.interpolatedWith(Colours::green, position * position));
//     g.fillPath(p);

//     g.setColour(Colours::lightgrey);
//     auto circleBounds = area.withWidth(h).translated((area.getWidth() - h) * position, 0.0f);
//     g.fillEllipse(circleBounds.reduced(1.0f));
//     return;
// }
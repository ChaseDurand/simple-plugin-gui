#include <juce_gui_basics/juce_gui_basics.h>
#include "CustomLookAndFeel.h"

RotaryDecibelSliderLookAndFeel::RotaryDecibelSliderLookAndFeel() : LookAndFeel_V4(){
    setColour (juce::Slider::thumbColourId, juce::Colours::black);
}

void RotaryDecibelSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, const float rotaryStartAngle, const float
    rotaryEndAngle, juce::Slider&){
    
    auto radius = (float) juce::jmin (width / 2, height / 2) - 6.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f + 3.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    float innerRadiusPercent = 0.76f;

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
    juce::Path p;
    auto pointerLength = radius * 0.8f;
    auto pointerThickness = 8.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour(juce::Colours::black);
    g.fillPath(p);

    // Pointer inner color
    p.clear();
    pointerLength = radius * 0.62f;
    pointerThickness = 4.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius+2.2f, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    g.setColour(juce::Colour(110, 221, 202));
    g.fillPath(p);

    // Outer arc background
    p.clear();
    p.startNewSubPath(centreX+1.3f*radius*sin(rotaryStartAngle), centreY-1.3f*radius*cos(rotaryStartAngle));
    p.addCentredArc(centreX, centreY, radius * 1.3f, radius * 1.3f,
        0.0f, rotaryStartAngle, rotaryEndAngle, false);
    g.setColour(juce::Colour(72, 72, 72));
    g.strokePath(p, juce::PathStrokeType(4.0f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

    // Outer arc foreground
    p.clear();
    p.startNewSubPath(centreX+1.3f*radius*sin(rotaryStartAngle), centreY-1.3f*radius*cos(rotaryStartAngle));
    p.addCentredArc(centreX, centreY, radius * 1.3f, radius * 1.3f,
        0.0f, rotaryStartAngle, angle, false);
    g.setColour(juce::Colour(110, 221, 202));
    g.strokePath(p, juce::PathStrokeType(4.0f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));
}

juce::Label* RotaryDecibelSliderLookAndFeel::createSliderTextBox(juce::Slider& slider){
    auto* l = juce::LookAndFeel_V2::createSliderTextBox (slider);
    if (getCurrentColourScheme() == juce::LookAndFeel_V4::getGreyColourScheme() && (slider.getSliderStyle() == juce::Slider::LinearBar
                                                                               || slider.getSliderStyle() == juce::Slider::LinearBarVertical))
    {
        l->setColour (juce::Label::textColourId, juce::Colours::black.withAlpha (0.7f));
    }
    l->setColour (juce::Label::outlineColourId, juce::Colours::black.withAlpha(0.0f)); // Make label transparant
    return l;
}
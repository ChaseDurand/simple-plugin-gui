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
    float outerRadiusPercent = 0.94f;
    float outerRadius = button.getWidth() * outerRadiusPercent * 0.5f;
    float innerRadiusPercent = 0.76f;
    float innerRadius =  button.getWidth() * innerRadiusPercent * 0.5f;
    float switchRadiusPercent = 0.64f;
    float switchRadius = button.getWidth() * switchRadiusPercent * 0.5f;

    float offsetX = (button.getWidth() - outerRadius * 2.0f) * 0.5f;
    float offsetY = (button.getHeight() - outerRadius * 2.0f) * 0.5f;

    float offsetXinner = (button.getWidth() - (outerRadius * 2.0f) * innerRadiusPercent) * 0.5f;
    float offsetYinner = (button.getHeight() - (outerRadius * 2.0f) * innerRadiusPercent) * 0.5f;

    float centreX = x + 0.5 * w;
    float centreY = y + 0.5 * h;

    // Outer circle gradient
    g.setGradientFill(juce::ColourGradient(juce::Colour(80, 80, 80), centreX, y,
                                            juce::Colour(32, 32, 32), centreX, centreY + outerRadius, false));
    g.fillEllipse (offsetX, offsetY, (outerRadius * 2.0f), (outerRadius * 2.0f));

    // Outer circle outline
    g.setColour(juce::Colours::black);
    g.drawEllipse (offsetX, offsetY,
        (outerRadius * 2.0f), (outerRadius * 2.0f), 2.0f);


    // Inner circle outline
    g.setColour(juce::Colours::black);
    g.drawEllipse (button.getWidth() * 0.5f - innerRadius,
        button.getWidth() * 0.5f - innerRadius,
        innerRadius * 2.0f, innerRadius * 2.0f, 2.0f);

    // On/off LED
    float lightCircum = 16.0f;
    float lightRadius = lightCircum * 0.5f;
    float lightOffsetX = (button.getWidth() - lightCircum) * 0.5f;
    float lightOffsetY = 16.0f;
    if (ticked)
    {
        // Enabled
        // Inner circle
        g.setGradientFill(juce::ColourGradient(juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getHeight() * 0.8f,
                                            juce::Colour(20, 20, 20), button.getWidth() * 0.5,
                                            button.getHeight() * 0.5f, true));

        g.fillEllipse (button.getWidth() * 0.5f - innerRadius,
            button.getWidth() * 0.5f - innerRadius,
            innerRadius * 2.0f, innerRadius * 2.0f);
        // Switch gradient
        g.setGradientFill(juce::ColourGradient(juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f - innerRadius,
                                            juce::Colour(30, 30, 30), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f - 0.2 * innerRadius, false));
        g.fillEllipse (button.getWidth() * 0.5f - switchRadius,
                    button.getWidth() * 0.5f - innerRadius,
                    switchRadius * 2.0f, switchRadius * 2.0f);

        // Switch outline
        g.setGradientFill(juce::ColourGradient(juce::Colour(90, 90, 90), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f - innerRadius,
                                            juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f, false));
        g.drawEllipse(button.getWidth() * 0.5f - switchRadius,
                    button.getWidth() * 0.5f - innerRadius,
                    switchRadius * 2.0f, switchRadius * 2.0f, 1.0f);

        // Light color
        g.setGradientFill(juce::ColourGradient(juce::Colour(250, 100, 100),
            button.getWidth() * 0.5f + lightRadius * 0.2f,
            lightOffsetY + 0.5f * lightRadius,
            juce::Colour(170, 20, 10),  button.getWidth() * 0.5f - lightRadius,
            lightOffsetY + 1.5f * lightRadius, true));
        g.fillEllipse (lightOffsetX, lightOffsetY,
            lightCircum, lightCircum);
        // On/off LED outline
        g.setColour(juce::Colours::black);
        g.drawEllipse (lightOffsetX, lightOffsetY,
            lightCircum, lightCircum, 1.0f);
    }
    else{
        // Disabled
        // Inner circle
        g.setGradientFill(juce::ColourGradient(juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getHeight() * 0.2f,
                                            juce::Colour(20, 20, 20), button.getWidth() * 0.5,
                                            button.getHeight() * 0.5f, true));
        g.fillEllipse (button.getWidth() * 0.5f - innerRadius,
            button.getWidth() * 0.5f - innerRadius,
            innerRadius * 2.0f, innerRadius * 2.0f);
        // Switch gradient
        g.setGradientFill(juce::ColourGradient(juce::Colour(30, 30, 30), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f + 0.3f * innerRadius,
                                            juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f + innerRadius, false));
        g.fillEllipse (button.getWidth() * 0.5f - switchRadius,
                    button.getWidth() * 0.5f + innerRadius - 2.0f * switchRadius,
                    switchRadius * 2.0f, switchRadius * 2.0f);

        // Switch outline
        g.setGradientFill(juce::ColourGradient(juce::Colour(90, 90, 90), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f - innerRadius,
                                            juce::Colour(70, 70, 70), button.getWidth() * 0.5,
                                            button.getWidth() * 0.5f, false));
        g.drawEllipse(button.getWidth() * 0.5f - switchRadius,
                    button.getWidth() * 0.5f + innerRadius - 2.0f * switchRadius,
                    switchRadius * 2.0f, switchRadius * 2.0f, 1.0f);

        float lightOffOffset = 1.55f;
        float lightOffParalax = 0.9f;
        // Light color
        g.setGradientFill(juce::ColourGradient(juce::Colour(200, 26, 47),
            button.getWidth() * 0.5f + lightRadius * 0.2f,
            lightOffsetY + 0.5f * lightRadius,
            juce::Colour(80, 16, 8),  button.getWidth() * 0.5f - lightRadius,
            lightOffsetY + 1.5f * lightRadius, true));
        g.fillEllipse (lightOffsetX, lightOffsetY * lightOffOffset,
            lightCircum, lightCircum * lightOffParalax);
        // On/off LED outline
        g.setColour(juce::Colours::black);
        g.drawEllipse (lightOffsetX, lightOffsetY * lightOffOffset,
            lightCircum, lightCircum * lightOffParalax, 1.0f);
    }

    return;
}
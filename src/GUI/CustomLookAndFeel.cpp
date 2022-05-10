#include "CustomLookAndFeel.h"
#include "CustomColours.h"
#include "MuteButton.h"
#include "ChannelButton.h"

///////////////////////////////////////
//       Rotary Decibel Slider       //
///////////////////////////////////////

RotaryDecibelSliderLookAndFeel::RotaryDecibelSliderLookAndFeel() : LookAndFeel_V4()
{
    setColour (juce::Slider::thumbColourId, juce::Colours::black);
}

juce::Slider::SliderLayout RotaryDecibelSliderLookAndFeel::getSliderLayout(juce::Slider& slider)
{
    juce::Rectangle<int> localBounds = slider.getLocalBounds();
    juce::Slider::SliderLayout layout;
    
    int textBoxHeight = localBounds.getHeight() * 0.17;
    int textBoxOffsetVertical = (localBounds.getHeight() - textBoxHeight) * 0.49;
    
    int textBoxWidth = localBounds.getWidth() * 0.7;
    int textBoxOffsetHorizontal = (localBounds.getWidth() - textBoxWidth) * 0.5;

    layout.textBoxBounds = localBounds.withY(-1);
    layout.textBoxBounds.setY(textBoxOffsetVertical);
    layout.textBoxBounds.setHeight(textBoxHeight);
    layout.textBoxBounds.setX(textBoxOffsetHorizontal);
    layout.textBoxBounds.setWidth(textBoxWidth);
    layout.sliderBounds = localBounds;
    return layout;
}

void RotaryDecibelSliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y,
    int width, int height, float sliderPos, const float rotaryStartAngle, const float
    rotaryEndAngle, juce::Slider& /*slider*/)
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
    // TODO use colors from plugin editor or here, don't have both

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

ResizableTextBox* RotaryDecibelSliderLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    // TODO reduce size
    auto* textBox = new ResizableTextBox();
    textBox->setBounds(0, slider.getHeight() * 0.5f, slider.getWidth(), 16);
    textBox->setColour ( juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    return textBox; 
}

///////////////////////////////////////
//            Mute Button            //
///////////////////////////////////////

void MuteButtonLookAndFeel::drawTickBox (juce::Graphics& g, juce::Component& button, float x, float y,
    float w, float /*h*/, bool ticked, bool /*isEnabled*/,
    bool /*shouldDrawButtonAsHighlighted*/,
    bool /*shouldDrawButtonAsDown*/)
{
    float outerMargin = w * 0.03f;
    juce::Rectangle<float> bounds = juce::Rectangle<float>
        (x, y, button.getWidth(), button.getHeight()).reduced(outerMargin);
    float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f;
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

///////////////////////////////////////
//          Channel Button           //
///////////////////////////////////////

void ChannelButtonLookAndFeel::drawButtonText(juce::Graphics & g,
    juce::TextButton & button, bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    juce::Font font = button.getHeight() * 0.2f;
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                            : juce::TextButton::textColourOffId)
                    .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 10;

    const int fontHeight = juce::roundToInt (button.getHeight() * 0.07f);
    const int leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                        leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                        juce::Justification::centred, 1);
}

void ChannelButtonLookAndFeel::drawButtonBackground(juce::Graphics & g,
    juce::Button & button, const juce::Colour & backgroundColour,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{

    auto cornerSize = button.getWidth() * 0.4f;
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                      .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = baseColour.contrasting (shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour (baseColour);

    auto flatOnLeft   = button.isConnectedOnLeft();
    auto flatOnRight  = button.isConnectedOnRight();
    auto flatOnTop    = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    float outlineWidth = juce::jmax(1.0f, button.getWidth() * 0.04f);

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        juce::Path path;
        path.addRoundedRectangle (bounds.getX() + outlineWidth * 0.5f,
            bounds.getY() + outlineWidth * 0.5f,
            bounds.getWidth() - outlineWidth,
            bounds.getHeight() - outlineWidth,
            cornerSize, cornerSize,
            ! (flatOnLeft  || flatOnTop),
            ! (flatOnRight || flatOnTop),
            ! (flatOnLeft  || flatOnBottom),
            ! (flatOnRight || flatOnBottom));

        g.fillPath (path);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.strokePath (path, juce::PathStrokeType (outlineWidth));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (bounds, cornerSize, outlineWidth);
    }
}
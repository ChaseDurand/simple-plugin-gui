static void drawFocusOutline(const juce::Rectangle<float>& bounds, juce::Graphics& g, juce::Colour c, const int& pluginWidth){
    float minLen = juce::jmin(bounds.getHeight() * 0.5f, bounds.getWidth() * 0.5f);
    float len = juce::jmin(pluginWidth*0.02f, minLen);
    float thick = pluginWidth*0.01f;

    juce::Point<float> topLeft = bounds.getTopLeft();
    juce::Point<float> topRight = bounds.getTopRight();
    juce::Point<float> bottomLeft = bounds.getBottomLeft();
    juce::Point<float> bottomRight = bounds.getBottomRight();

    g.setColour(c);

    juce::Path topLeftPath;
    topLeftPath.startNewSubPath (topLeft);
    topLeftPath.lineTo (topLeft.x, topLeft.y + len);
    topLeftPath.startNewSubPath (topLeft);
    topLeftPath.lineTo (topLeft.x + len, topLeft.y);
    g.strokePath (topLeftPath, juce::PathStrokeType (thick));

    juce::Path topRightPath;
    topRightPath.startNewSubPath (topRight);
    topRightPath.lineTo (topRight.x, topRight.y + len);
    topRightPath.startNewSubPath (topRight);
    topRightPath.lineTo (topRight.x - len, topRight.y);
    g.strokePath (topRightPath, juce::PathStrokeType (thick));

    juce::Path bottomLeftPath;
    bottomLeftPath.startNewSubPath (bottomLeft);
    bottomLeftPath.lineTo (bottomLeft.x, bottomLeft.y - len);
    bottomLeftPath.startNewSubPath (bottomLeft);
    bottomLeftPath.lineTo (bottomLeft.x + len, bottomLeft.y);
    g.strokePath (bottomLeftPath, juce::PathStrokeType (thick));

    juce::Path bottomRightPath;
    bottomRightPath.startNewSubPath (bottomRight);
    bottomRightPath.lineTo (bottomRight.x, bottomRight.y - len);
    bottomRightPath.startNewSubPath (bottomRight);
    bottomRightPath.lineTo (bottomRight.x - len, bottomRight.y);
    g.strokePath (bottomRightPath, juce::PathStrokeType (thick));
    return;
}
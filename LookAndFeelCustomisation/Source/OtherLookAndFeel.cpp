/*
  ==============================================================================

    OtherLookAndFeel.cpp
    Created: 29 May 2021 8:40:30pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OtherLookAndFeel.h"

//==============================================================================
OtherLookAndFeel::OtherLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

void OtherLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centerX = (float)x + (float)width * 0.5f;
    auto centerY = (float)y + (float)height * 0.5f;
    auto rx = centerX - radius;
    auto ry = centerY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    g.setColour(juce::Colours::orange);
    g.fillEllipse(rx, ry, rw, rw);
    
    g.setColour(juce::Colours::red);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);
    
    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    
    g.setColour(juce::Colours::yellow);
    g.fillPath(p);
}

void OtherLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool, bool isButtonDown)
{
    auto buttonArea = button.getLocalBounds();
    auto edge = 4;
    
    buttonArea.removeFromLeft(edge);
    buttonArea.removeFromTop(edge);
    
    g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
    g.fillRect(buttonArea);
    
    auto offset = isButtonDown ? -edge / 2 : -edge;
    buttonArea.translate(offset, offset);
    
    g.setColour(backgroundColour);
    g.fillRect(buttonArea);
    
}

void OtherLookAndFeel::drawButtonText(juce::Graphics &g, juce::TextButton &button, bool isMouseOverButton, bool isButtonDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
                                  : juce::TextButton::textColourOffId)
                .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
    
    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
    
    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;
    
    auto edge = 4;
    auto offset = isButtonDown ? edge / 2 : 0;
    
    if (textWidth > 0) {
        g.drawFittedText(button.getButtonText(), leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge, juce::Justification::centred, 2);
    }
    
}

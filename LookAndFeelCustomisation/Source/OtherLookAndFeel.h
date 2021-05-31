/*
  ==============================================================================

    OtherLookAndFeel.h
    Created: 29 May 2021 8:40:30pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OtherLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &) override;
    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool, bool isButtonDown) override;
    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OtherLookAndFeel)
};

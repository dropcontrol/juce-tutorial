/*
  ==============================================================================

    DecibelSliderComponent.h
    Created: 16 Apr 2021 5:37:23pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DecibelSliderComponent  : public juce::Slider
{
public:
    DecibelSliderComponent();
    ~DecibelSliderComponent() override;

//    void paint (juce::Graphics&) override;
//    void resized() override;
    
    double getValueFromText (const juce::String &text) override;
    
    juce::String getTextFromValue (double value) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecibelSliderComponent)
};

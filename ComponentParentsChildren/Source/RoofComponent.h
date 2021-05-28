/*
  ==============================================================================

    RoofComponent.h
    Created: 28 May 2021 6:21:59pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class RoofComponent  : public juce::Component
{
public:
    RoofComponent();
    ~RoofComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RoofComponent)
};

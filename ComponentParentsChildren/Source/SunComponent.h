/*
  ==============================================================================

    SunComponent.h
    Created: 28 May 2021 6:32:35pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SunComponent  : public juce::Component
{
public:
    SunComponent();
    ~SunComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SunComponent)
};

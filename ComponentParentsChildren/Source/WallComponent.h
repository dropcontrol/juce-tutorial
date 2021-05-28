/*
  ==============================================================================

    WallComponent.h
    Created: 28 May 2021 6:21:48pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WallComponent  : public juce::Component
{
public:
    WallComponent();
    ~WallComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WallComponent)
};

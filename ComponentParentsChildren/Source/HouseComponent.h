/*
  ==============================================================================

    HouseComponent.h
    Created: 28 May 2021 6:14:22pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WallComponent.h";
#include "RoofComponent.h";

//==============================================================================
/*
*/
class HouseComponent  : public juce::Component
{
public:
    HouseComponent();
    ~HouseComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    WallComponent wall;
    RoofComponent roof;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HouseComponent)
};

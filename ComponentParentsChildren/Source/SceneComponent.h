/*
  ==============================================================================

    SceneComponent.h
    Created: 28 May 2021 6:07:51pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FloorComponent.h"
#include "HouseComponent.h"
#include "SunComponent.h"

//==============================================================================
/*
*/
class SceneComponent  : public juce::Component
{
public:
    SceneComponent();
    ~SceneComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    FloorComponent floor;
    HouseComponent house;
    HouseComponent smallHouse;
    SunComponent sun;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SceneComponent)
};

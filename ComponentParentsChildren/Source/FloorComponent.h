/*
  ==============================================================================

    FloorComponent.h
    Created: 28 May 2021 6:14:08pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FloorComponent  : public juce::Component
{
public:
    FloorComponent();
    ~FloorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloorComponent)
};

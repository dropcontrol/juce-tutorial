/*
  ==============================================================================

    SunComponent.cpp
    Created: 28 May 2021 6:32:35pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SunComponent.h"

//==============================================================================
SunComponent::SunComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SunComponent::~SunComponent()
{
}

void SunComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour(juce::Colours::yellow);
    auto lineThickness = 3.0f;
    g.drawEllipse(lineThickness * 0.5f,
                  lineThickness * 0.5f,
                  (float)getWidth() - lineThickness * 2,
                  (float)getHeight() - lineThickness * 2,
                  lineThickness);
}

void SunComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

/*
  ==============================================================================

    FloorComponent.cpp
    Created: 28 May 2021 6:14:08pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FloorComponent.h"

//==============================================================================
FloorComponent::FloorComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FloorComponent::~FloorComponent()
{
}

void FloorComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour(juce::Colours::green);
    g.drawLine(0.0f, (float)getHeight() / 2.0f, (float)getWidth(), (float)getHeight() / 2.0f, 5.0f);
}

void FloorComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

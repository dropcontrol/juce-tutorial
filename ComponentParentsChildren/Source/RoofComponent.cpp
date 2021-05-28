/*
  ==============================================================================

    RoofComponent.cpp
    Created: 28 May 2021 6:21:59pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "RoofComponent.h"

//==============================================================================
RoofComponent::RoofComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

RoofComponent::~RoofComponent()
{
}

void RoofComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.setColour (juce::Colours::red);
     
    juce::Path roof;
    roof.addTriangle (0.0f, (float) getHeight(), (float) getWidth(), (float) getHeight(), (float) getWidth() / 2.0f, 0.0f);
    g.fillPath (roof);
}

void RoofComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

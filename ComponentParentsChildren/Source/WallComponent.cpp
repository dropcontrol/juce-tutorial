/*
  ==============================================================================

    WallComponent.cpp
    Created: 28 May 2021 6:21:48pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WallComponent.h"

//==============================================================================
WallComponent::WallComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WallComponent::~WallComponent()
{
}

void WallComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillCheckerBoard(getLocalBounds().toFloat(), 30, 10, juce::Colours::sandybrown, juce::Colours::saddlebrown);
}

void WallComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

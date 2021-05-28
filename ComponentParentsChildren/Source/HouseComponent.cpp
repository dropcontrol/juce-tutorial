/*
  ==============================================================================

    HouseComponent.cpp
    Created: 28 May 2021 6:14:22pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HouseComponent.h"

//==============================================================================
HouseComponent::HouseComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(wall);
    addAndMakeVisible(roof);

}

HouseComponent::~HouseComponent()
{
}

void HouseComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

}

void HouseComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    auto separation = juce::jlimit(2, 10, getHeight() / 20);
    
    roof.setBounds(0, 0, getWidth(), (int)(getHeight() * 0.2) - separation / 2);
    wall.setBounds(0, (int)(getHeight() * 0.20) + separation / 2, getWidth(), (int)(getHeight() * 0.80) - separation);

}

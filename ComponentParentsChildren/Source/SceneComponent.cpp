/*
  ==============================================================================

    SceneComponent.cpp
    Created: 28 May 2021 6:07:51pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SceneComponent.h"

//==============================================================================
SceneComponent::SceneComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(floor);
    addAndMakeVisible(house);
    addAndMakeVisible(smallHouse);
    addAndMakeVisible(sun);
}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::lightblue);
}

void SceneComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    floor.setBounds(10, 297, 580, 5);
    house.setBounds(300, 70, 200, 220);
    smallHouse.setBounds(50, 50, 50, 50);
    sun.setBounds(530, 10, 60, 60);

}

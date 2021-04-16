/*
  ==============================================================================

    DecibelSliderComponent.cpp
    Created: 16 Apr 2021 5:37:23pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DecibelSliderComponent.h"

//==============================================================================
DecibelSliderComponent::DecibelSliderComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

DecibelSliderComponent::~DecibelSliderComponent()
{
}

double DecibelSliderComponent::getValueFromText(const juce::String &text)
{
    auto minusInfinitydB = -100.0;
    auto decibelText = text.upToFirstOccurrenceOf("db", false, false).trim();
    return decibelText.equalsIgnoreCase("-INF") ? minusInfinitydB : decibelText.getDoubleValue();
    
}

juce::String DecibelSliderComponent::getTextFromValue(double value)
{
    return juce::Decibels::toString(value);
}

//void DecibelSliderComponent::paint (juce::Graphics& g)
//{
//    /* This demo code just fills the component's background and
//       draws some placeholder text to get you started.
//
//       You should replace everything in this method with your own
//       drawing code..
//    */
//
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//
//    g.setColour (juce::Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
//
//    g.setColour (juce::Colours::white);
//    g.setFont (14.0f);
//    g.drawText ("DecibelSliderComponent", getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
//}

//void DecibelSliderComponent::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}

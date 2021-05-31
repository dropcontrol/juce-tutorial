#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (300, 200);
//    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);
//    otherLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    
    setLookAndFeel(&otherLookAndFeel);

    dial1.setSliderStyle(juce::Slider::Rotary);
    dial1.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
//    dial1.setLookAndFeel(&otherLookAndFeel);
    addAndMakeVisible(dial1);
    
    dial2.setSliderStyle(juce::Slider::Rotary);
    dial2.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(dial2);
    
    button1.setButtonText("Button 1");
    addAndMakeVisible(button1);
    
    button2.setButtonText("Button 2");
    addAndMakeVisible(button2);
    

}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    auto border = 4;
    auto area = getLocalBounds();
    
    auto dialArea = area.removeFromTop(area.getHeight() / 2);
    dial1.setBounds(dialArea.removeFromLeft(dialArea.getWidth() / 2).reduced(border));
    dial2.setBounds(dialArea.reduced(border));
    
    auto buttonHeight = 30;
    button1.setBounds(area.removeFromTop(buttonHeight).reduced(border));
    button2.setBounds(area.removeFromTop(buttonHeight).reduced(border));
}

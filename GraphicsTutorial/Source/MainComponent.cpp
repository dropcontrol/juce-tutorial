#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::lightblue);

//    g.setFont (juce::Font (20.0f));
//    juce::Font mainComponentFont("Times New Roman", 20.0f, juce::Font::italic);
//    g.setFont(mainComponentFont);
    g.setFont(juce::Font("Times New Roman", 20.0f, juce::Font::italic));
    g.setColour (juce::Colours::darkblue);
    g.drawText ("Hello World!", 20, 40, 200, 40, juce::Justification::centred, true);
    
    g.setColour(juce::Colours::green);
    g.drawLine(10, 300, 590, 300, 5);
    
    juce::Rectangle<float> house(300, 120, 200, 170);
//    g.setColour(juce::Colours::sandybrown);
//    g.fillRect(house);
//    g.drawRoundedRectangle(house, 10.0, 3.0);
//    g.fillRoundedRectangle(house, 10.0);
    g.fillCheckerBoard(house, 30, 10, juce::Colours::sandybrown, juce::Colours::saddlebrown);
    
//    g.setColour(juce::Colours::yellow);
//    g.drawEllipse(getWidth() - 70, 10, 60, 60, 3);
    
    drawElipseWithColors(g, 560, 40, 30, juce::Colours::yellow);
    
    g.setColour(juce::Colours::red);
    juce::Path roof;
    roof.addTriangle(300, 110, 500, 110, 400, 70);
    g.fillPath(roof);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::drawElipseWithColors(juce::Graphics& g, int x, int y, int radius, juce::Colour color) {
    g.setColour(color);
    g.drawEllipse(x - radius, y - radius, radius * 2, radius * 2, 3);
}

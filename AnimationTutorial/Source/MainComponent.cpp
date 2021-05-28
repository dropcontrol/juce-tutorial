#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    setFramesPerSecond (60); // This sets the frequency of the update calls.
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    
    g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));
    auto numberOfDots = 15;
    juce::Path spinePath;
    
    for (auto i = 0; i < numberOfDots; ++i) {
        auto radius  = 100 + 10 *std::sin(getFrameCounter() * 0.1f * i * 0.5f);
        
        juce::Point<float> p((float)getWidth() / 2.0f + 1.5f * (float)radius * std::sin((float)getFrameCounter() * 0.02f + (float)i * 0.12f),
                             (float)getHeight() / 2.0f + 1.0f * (float)radius * std::cos((float)getFrameCounter() * 0.04f + (float)i * 0.12f));
        
        g.fillEllipse(p.x - i, p.y - i, 2.0f + 2.0f * i, 2.0f + 2.0f *i);

        if (i == 0) {
            spinePath.startNewSubPath(p);
        } else {
            spinePath.lineTo(p);
        }
    }
    g.strokePath(spinePath, juce::PathStrokeType(4.0f));
    
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

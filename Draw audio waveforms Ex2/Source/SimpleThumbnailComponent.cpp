/*
  ==============================================================================

    SimpleThumbnailComponent.cpp
    Created: 14 Apr 2021 6:53:12pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SimpleThumbnailComponent.h"

//==============================================================================
SimpleThumbnailComponent::SimpleThumbnailComponent(int sourceSamplesPerThumbnailSample,
                                                   juce::AudioFormatManager &formatManager,
                                                   juce::AudioThumbnailCache &thumbnailCache)
    : thumbnail(sourceSamplesPerThumbnailSample, formatManager, thumbnailCache)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    thumbnail.addChangeListener(this);
}

SimpleThumbnailComponent::~SimpleThumbnailComponent()
{
}

void SimpleThumbnailComponent::paint (juce::Graphics& g)
{
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g);
    else
        paintIfFileLoaded(g);
}

void SimpleThumbnailComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void SimpleThumbnailComponent::setFile(const juce::File &file)
{
    thumbnail.setSource(new juce::FileInputSource(file));
}

void SimpleThumbnailComponent::paintIfNoFileLoaded(juce::Graphics &g)
{
    g.setColour(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleThumbnailComponent::paintIfFileLoaded(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white);
    
    g.setColour(juce::Colours::red);
    
    auto audioLength = (float)thumbnail.getTotalLength();
    thumbnail.drawChannels(g, getLocalBounds(), 0.0, audioLength, 1.0f);
}

// callback
void SimpleThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &thumbnail) thumbnailChanged();
}

void SimpleThumbnailComponent::thumbnailChanged()
{
    repaint();
}

//==============================================================================
SimplePositionOverlay::SimplePositionOverlay(juce::AudioTransportSource &transportSourceToUse)
    : transportSource(transportSourceToUse)
{
    startTimer(40);
}

void SimplePositionOverlay::paint(juce::Graphics &g)
{
    auto duration = (float)transportSource.getLengthInSeconds();
    
    if(duration > 0.0)
    {
        auto audioPosition = (float)transportSource.getCurrentPosition();
        auto drawPosition = (audioPosition / duration) * (float)getWidth();
        
        g.setColour(juce::Colours::green);
        g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
    }
}

void SimplePositionOverlay::mouseDown(const juce::MouseEvent &event)
{
    auto duration = transportSource.getLengthInSeconds();
    
    if (duration > 0.0)
    {
        auto clickPosition = event.position.x;
        auto audioPosition = (clickPosition / (float)getWidth()) * duration;
        
        transportSource.setPosition(audioPosition);
    }
}

void SimplePositionOverlay::timerCallback()
{
    repaint();
}

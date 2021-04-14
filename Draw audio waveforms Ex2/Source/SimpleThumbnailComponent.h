/*
  ==============================================================================

    SimpleThumbnailComponent.h
    Created: 14 Apr 2021 6:53:12pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SimpleThumbnailComponent  : public juce::Component, private juce::ChangeListener
{
public:
    SimpleThumbnailComponent(int sourceSamplesPerThumbnailSample,
                             juce::AudioFormatManager &formatManager,
                             juce::AudioThumbnailCache &thumbnailCache);
    
    ~SimpleThumbnailComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setFile(const juce::File &file);
    void paintIfNoFileLoaded(juce::Graphics& g);
    void paintIfFileLoaded(juce::Graphics& g);
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    

private:
    void thumbnailChanged();

    juce::AudioThumbnail thumbnail;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleThumbnailComponent)
};

//------------------------------------------------------------------------------
class SimplePositionOverlay : public juce::Component, private juce::Timer
{
public:
    SimplePositionOverlay(juce::AudioTransportSource &transportSourceToUse);
    
    void paint (juce::Graphics &g) override;
    void mouseDown(const juce::MouseEvent &event) override;

private:
    void timerCallback() override;
    
    juce::AudioTransportSource &transportSource;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplePositionOverlay)
};

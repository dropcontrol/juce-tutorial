#pragma once

#include <JuceHeader.h>
#include "SimpleThumbnailComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    //==============================================================================
    // Your private member variables go here...
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    
    SimpleThumbnailComponent thumbnailComp;
    SimplePositionOverlay positionOverlay;
    
    void changeState(TransportState newState);
    void transportSourceChanged();
    
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;

    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


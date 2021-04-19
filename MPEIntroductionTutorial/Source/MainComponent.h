#pragma once

#include <JuceHeader.h>
#include "VisualiserComponent.h"
#include "MPEDemoSynthVoice.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component,
                       private juce::AudioIODeviceCallback,
                       private juce::MidiInputCallback
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
//    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
//    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
//    void releaseResources() override;
    
    void audioDeviceStopped() override {}


    //==============================================================================
//    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void audioDeviceAboutToStart(juce::AudioIODevice *device) override;
    void audioDeviceIOCallback(const float ** /*inputChannelDelta*/, int /*numInputChannels*/, float **outputChannelData, int numOutputChannels, int numSamples) override;

private:
    //==============================================================================
    // Your private member variables go here...
    void handleIncomingMidiMessage(juce::MidiInput * /*source*/, const juce::MidiMessage &message) override;
    
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioDeviceSelectorComponent audioSetupComp;
    
    VisualiserComponent visualiserComp;
    juce::Viewport visualiserViewport;
    
    juce::MPEInstrument visualiserInstrument;
    juce::MPESynthesiser synth;
    juce::MidiMessageCollector midiCollector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#pragma once

#include <JuceHeader.h>
//#include "SineOscillator.h"
#include "WavetableOscillator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::Timer
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
    
    void timerCallback() override;
    void createWavetable();
    

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Label cpuUsageLabel;
    juce::Label cpuUsageText;
    
    const unsigned int tableSize = 1 << 7; // 2の7乗で128
    float level = 0.0f;
    
    juce::AudioSampleBuffer sineTable;
//    juce::OwnedArray<SineOscillator> oscillators;
    juce::OwnedArray<WavetableOscillator> oscillators;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

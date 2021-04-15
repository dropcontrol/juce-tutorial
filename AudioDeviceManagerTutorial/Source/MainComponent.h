#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent,
                        public juce::ChangeListener,
                        private juce::Timer
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

private:
    //==============================================================================
    // Your private member variables go here...
    void changeListenerCallback(juce::ChangeBroadcaster*) override;
    void dumpDeviceInfo();
    void logMessage(const juce::String &m);
    void timerCallback() override;
    
    juce::AudioDeviceSelectorComponent audioSetupComp;
    juce::TextEditor diagnosticsBox;
    juce::Label cpuUsageLabel;
    juce::Label cpuUsageText;
    juce::Random randam;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

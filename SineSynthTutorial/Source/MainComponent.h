#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
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
    
    void updateAngleDelta();

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Slider frequencySlider;
    juce::Slider levelSlider;
    double currentSampleRate = 0.0;
    double currentAngle = 0.0;
    double angleDelta = 0.0;
//    double level;
    double currentFrequency = 500.0;
    double targetFrequency = 500.0;
    double currentLevel = 0.1f;
    double targetLevel = 0.1f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

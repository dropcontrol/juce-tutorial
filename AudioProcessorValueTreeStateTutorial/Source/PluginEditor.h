/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioProcessorValueTreeStateTutorialAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    
    enum {
        paramControlHeight = 40,
        paramLabelWidth = 80,
        paramSliderWidth = 300
    };
    
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAtachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    AudioProcessorValueTreeStateTutorialAudioProcessorEditor (AudioProcessorValueTreeStateTutorialAudioProcessor&, juce::AudioProcessorValueTreeState &vts);
    ~AudioProcessorValueTreeStateTutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioProcessorValueTreeStateTutorialAudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState &valueTreeState;
    
    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<SliderAtachment> gainAttachment;
    
    juce::ToggleButton invertButton;
    std::unique_ptr<ButtonAttachment> invertAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorValueTreeStateTutorialAudioProcessorEditor)
};

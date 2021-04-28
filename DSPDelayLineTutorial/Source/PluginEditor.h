/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ScopeComponent.h"

//==============================================================================
/**
*/
class DSPIntroductionTutorialAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DSPIntroductionTutorialAudioProcessorEditor (DSPIntroductionTutorialAudioProcessor&);
    ~DSPIntroductionTutorialAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DSPIntroductionTutorialAudioProcessor& audioProcessor;
    juce::MidiKeyboardState midiKeyboardState;
    juce::MidiKeyboardComponent midiKeyboardComponent {
        midiKeyboardState,
        juce::MidiKeyboardComponent::horizontalKeyboard
    };
    ScopeComponent<float> scopeComponet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPIntroductionTutorialAudioProcessorEditor)
};


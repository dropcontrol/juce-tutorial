/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "ProcessorBase.h"
#include "OscillatorProcessor.h"
#include "GainProcessor.h"
#include "FilterProcessor.h"

//==============================================================================
/**
*/
class AudioProcessorGraphTutorialAudioProcessor  : public juce::AudioProcessor
{
public:
    
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;
    
    //==============================================================================
    AudioProcessorGraphTutorialAudioProcessor();
    ~AudioProcessorGraphTutorialAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    void initialiseGraph();
    void updateGraph();
    void connectAudioNodes();
    void connectMidiNodes();
    
    
    //==============================================================================
    
    juce::StringArray processorChoice { "Empty", "Oscillator", "Gain", "Filter" };
    
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;
    
    juce::AudioParameterBool *muteInput;
    
    juce::AudioParameterChoice *processorSlot1;
    juce::AudioParameterChoice *processorSlot2;
    juce::AudioParameterChoice *processorSlot3;
    
    juce::AudioParameterBool *bypassSLot1;
    juce::AudioParameterBool *bypassSLot2;
    juce::AudioParameterBool *bypassSLot3;
    
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;
    
    Node::Ptr slot1Node;
    Node::Ptr slot2Node;
    Node::Ptr slot3Node;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProcessorGraphTutorialAudioProcessor)
};

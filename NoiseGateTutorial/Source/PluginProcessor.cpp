/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoiseGateTutorialAudioProcessor::NoiseGateTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo())
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo())
                       .withInput("Sidechain", juce::AudioChannelSet::stereo())
                     #endif
                       )
#endif
{
    addParameter(threshold = new juce::AudioParameterFloat("threshold", "Threshold", 0.0f, 1.0f, 0.5f));
    addParameter(alpha = new juce::AudioParameterFloat("alpha", "Alpha", 0.0f, 1.0f, 0.8f));
}

NoiseGateTutorialAudioProcessor::~NoiseGateTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String NoiseGateTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseGateTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGateTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGateTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoiseGateTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseGateTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoiseGateTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseGateTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoiseGateTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoiseGateTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoiseGateTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lowPassCoeff = 0.0f;
    sampleCountDown = 0;
}

void NoiseGateTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseGateTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NoiseGateTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    auto mainInputOutput = getBusBuffer(buffer, true, 0);
    auto sideChainInput = getBusBuffer(buffer, true, 1);
    
    auto alphaCopy = alpha->get();
    auto thresholdCopy = threshold->get();
    
    for (auto j = 0; j < buffer.getNumSamples(); ++j) {
        auto mixedSamples = 0.0f;
        
        for (auto i = 0; i < sideChainInput.getNumChannels(); ++i) {
            mixedSamples += sideChainInput.getReadPointer(i) [j];
        }
        
        mixedSamples /= static_cast<float>(sideChainInput.getNumChannels());
        lowPassCoeff = (alphaCopy * lowPassCoeff) + ((1.0f - alphaCopy) * mixedSamples);
        
        if (lowPassCoeff >= thresholdCopy) {
            sampleCountDown = (int)getSampleRate();
        }
        
        for (auto i = 0; i < mainInputOutput.getNumChannels(); ++i) {
            *mainInputOutput.getWritePointer(i, j) = sampleCountDown > 0 ? *mainInputOutput.getReadPointer(i, j) : 0.0f;
        }
        
        if (sampleCountDown > 0) {
            --sampleCountDown;
        }
    }
}

//==============================================================================
bool NoiseGateTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoiseGateTutorialAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void NoiseGateTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(*threshold);
    stream.writeFloat(*alpha);
    
}

void NoiseGateTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    
    threshold->setValueNotifyingHost(stream.readFloat());
    alpha->setValueNotifyingHost(stream.readFloat());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateTutorialAudioProcessor();
}

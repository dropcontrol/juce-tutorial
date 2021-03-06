/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ArpeggiatorTutorialAudioProcessor::ArpeggiatorTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
//                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
//                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(speed = new juce::AudioParameterFloat ("Speed", "Arpeggiator Speed", 0.0, 1.0, 0.5));
}

ArpeggiatorTutorialAudioProcessor::~ArpeggiatorTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String ArpeggiatorTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ArpeggiatorTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ArpeggiatorTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ArpeggiatorTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ArpeggiatorTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ArpeggiatorTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ArpeggiatorTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ArpeggiatorTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ArpeggiatorTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void ArpeggiatorTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ArpeggiatorTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    notes.clear();
    currentNote = 0;
    lastNoteValue = -1;
    time = 0;
    rate = static_cast<float>(sampleRate);
}

void ArpeggiatorTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ArpeggiatorTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
//  #else
//    // This is the place where you check if the layout is supported.
//    // In this template code we only support mono or stereo.
//    // Some plugin hosts, such as certain GarageBand versions, will only
//    // load plugins that support stereo bus layouts.
//    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
//     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
//        return false;
//
//    // This checks if the input layout matches the output layout
//   #if ! JucePlugin_IsSynth
//    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
//        return false;
//   #endif
//
//    return true;
  #endif
}
#endif

void ArpeggiatorTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    
    jassert(buffer.getNumChannels() == 0);
    
    auto numSamples = buffer.getNumSamples();
    
    auto noteDuration = static_cast<int>(std::ceil(rate * 0.25f * (0.1f - (*speed))));
    
    for (const auto metadata : midiMessages) {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn()) notes.add(msg.getNoteNumber());
        else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
    }
    midiMessages.clear();
    
    if ((time + numSamples) >= noteDuration) {
        auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));
        
        if (lastNoteValue > 0) {
            midiMessages.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
            lastNoteValue = -1;
        }
        
        if (notes.size() > 0) {
            currentNote = (currentNote + 1) % notes.size();
            lastNoteValue = notes[currentNote];
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
        }
    }
    time = (time + numSamples) % noteDuration;
}

//==============================================================================
bool ArpeggiatorTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ArpeggiatorTutorialAudioProcessor::createEditor()
{
    return new ArpeggiatorTutorialAudioProcessorEditor (*this);
}

//==============================================================================
void ArpeggiatorTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream(destData, true).writeFloat(*speed);
}

void ArpeggiatorTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    speed->setValueNotifyingHost(juce::MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArpeggiatorTutorialAudioProcessor();
}

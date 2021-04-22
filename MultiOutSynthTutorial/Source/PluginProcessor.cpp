/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiOutSynthTutorialAudioProcessor::MultiOutSynthTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output #1", juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output #2",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #3",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #4",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #5",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #6",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #7",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #8",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #9",  juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #10", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #11", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #12", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #13", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #14", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #15", juce::AudioChannelSet::stereo(), false)
                       .withOutput ("Output #16", juce::AudioChannelSet::stereo(), false)
                     #endif
                       )
#endif
{
    formatManager.registerBasicFormats();
    
    for (auto midiChannel = 0; midiChannel < maxMidiChannel; ++midiChannel) {
        synth.add(new juce::Synthesiser());
        
        for (auto i = 0; i < maxNumberOfVoice; ++i) {
            synth[midiChannel]->addVoice(new juce::SamplerVoice());
        }
    }
    
    loadNewSample(juce::MemoryBlock(singing_ogg, singing_oggSize));
}

MultiOutSynthTutorialAudioProcessor::~MultiOutSynthTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String MultiOutSynthTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultiOutSynthTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultiOutSynthTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultiOutSynthTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultiOutSynthTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultiOutSynthTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultiOutSynthTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultiOutSynthTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultiOutSynthTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultiOutSynthTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultiOutSynthTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    juce::ignoreUnused(samplesPerBlock);
    
    for (auto midiChannel = 0; midiChannel < maxMidiChannel; ++midiChannel) {
        synth[midiChannel]->setCurrentPlaybackSampleRate(sampleRate);
    }
    
    
}

void MultiOutSynthTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiOutSynthTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MultiOutSynthTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        auto busCount = getBusCount(false);
        
        for (auto busNr = 0; busNr < busCount; ++busNr) {
            auto midiChannelBuffer = filterMidiMessageesForChannel(midiMessages, busNr + 1);
            auto audioBusBuffer = getBusBuffer(buffer, false, busNr);
            
            synth[busNr]->renderNextBlock(audioBusBuffer, midiChannelBuffer, 0, audioBusBuffer.getNumSamples());
        }
    }
}

//==============================================================================
bool MultiOutSynthTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiOutSynthTutorialAudioProcessor::createEditor()
{
//    return new MultiOutSynthTutorialAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MultiOutSynthTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultiOutSynthTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiOutSynthTutorialAudioProcessor();
}


void MultiOutSynthTutorialAudioProcessor::loadNewSample(const juce::MemoryBlock &sampleData)
{
    auto soundBuffer = std::make_unique<juce::MemoryInputStream>(sampleData, false);
    std::unique_ptr<juce::AudioFormatReader> formatReader(formatManager.findFormatForFileExtension("ogg")->createReaderFor(soundBuffer.release(), true));
    
    juce::BigInteger midiNotes;
    midiNotes.setRange(0, 126, true);
    juce::SynthesiserSound::Ptr newSound = new juce::SamplerSound("Voice", *formatReader, midiNotes, 0x40, 0.0, 0.0, 10.0);
    
    for (auto channel = 0; channel < maxMidiChannel; ++channel) {
        synth[channel]->removeSound(0);
    }
    sound = newSound;
    
    for (auto channel = 0; channel < maxMidiChannel; ++channel) {
        synth[channel]->addSound(sound);
    }
    
}

bool MultiOutSynthTutorialAudioProcessor::canAddBus(bool isInput) const
{
    return (! isInput && getBusCount(false) < maxMidiChannel);
}

bool MultiOutSynthTutorialAudioProcessor::canRemoveBus(bool isInput) const
{
    return (! isInput && getBusCount(false) > 1);
}

juce::MidiBuffer MultiOutSynthTutorialAudioProcessor::filterMidiMessageesForChannel(const juce::MidiBuffer &input, int channel)
{
    juce::MidiBuffer output;
    
    for (auto metadata : input) {
        auto message = metadata.getMessage();
        
        if (message.getChannel() == channel) {
            output.addEvent(message, metadata.samplePosition);
        }
    }
    
    return output;
}




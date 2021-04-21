/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
AudioProcessorGraphTutorialAudioProcessor::AudioProcessorGraphTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        mainProcessor(new juce::AudioProcessorGraph()),
        muteInput(new juce::AudioParameterBool("mute", "Mute Input", true)),
        processorSlot1(new juce::AudioParameterChoice("slot1", "Slot 1", processorChoice, 0)),
        processorSlot2(new juce::AudioParameterChoice("slot2", "Slot 2", processorChoice, 0)),
        processorSlot3(new juce::AudioParameterChoice("slot3", "Slot 3", processorChoice, 0)),
        bypassSLot1(new juce::AudioParameterBool("bypass1", "Bypass 1", false)),
        bypassSLot2(new juce::AudioParameterBool("bypass2", "Bypass 2", false)),
        bypassSLot3(new juce::AudioParameterBool("bypass3", "Bypass 3", false))
#endif
{
    addParameter(muteInput);
    
    addParameter(processorSlot1);
    addParameter(processorSlot2);
    addParameter(processorSlot3);
    
    addParameter(bypassSLot1);
    addParameter(bypassSLot2);
    addParameter(bypassSLot3);
}

AudioProcessorGraphTutorialAudioProcessor::~AudioProcessorGraphTutorialAudioProcessor()
{
}

//==============================================================================
const juce::String AudioProcessorGraphTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioProcessorGraphTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioProcessorGraphTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioProcessorGraphTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioProcessorGraphTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioProcessorGraphTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioProcessorGraphTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioProcessorGraphTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioProcessorGraphTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioProcessorGraphTutorialAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioProcessorGraphTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(), getMainBusNumOutputChannels(), sampleRate, samplesPerBlock);
    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    initialiseGraph();
}

void AudioProcessorGraphTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    mainProcessor->releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioProcessorGraphTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled()) {
        return false;
    }
    
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

void AudioProcessorGraphTutorialAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        
        for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) {
            buffer.clear(i, 0, buffer.getNumSamples());
        }
        updateGraph();
        mainProcessor->processBlock(buffer, midiMessages);
    }
}

//==============================================================================
bool AudioProcessorGraphTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioProcessorGraphTutorialAudioProcessor::createEditor()
{
    return new AudioProcessorGraphTutorialAudioProcessorEditor (*this);
}

//==============================================================================
void AudioProcessorGraphTutorialAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioProcessorGraphTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessorGraphTutorialAudioProcessor();
}

void AudioProcessorGraphTutorialAudioProcessor::initialiseGraph()
{
    mainProcessor->clear();
    
    audioInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::audioOutputNode));
    midiInputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiInputNode));
    midiOutputNode = mainProcessor->addNode(std::make_unique<AudioGraphIOProcessor>(AudioGraphIOProcessor::midiOutputNode));
    
    connectAudioNodes();
    connectMidiNodes();
}

void AudioProcessorGraphTutorialAudioProcessor::updateGraph()
{
    bool hasChanged = false;
    
    juce::Array<juce::AudioParameterChoice*> choices { processorSlot1, processorSlot2, processorSlot3} ;
    juce::Array<juce::AudioParameterBool*> bypasses { bypassSLot1, bypassSLot2, bypassSLot3 };
    
    juce::ReferenceCountedArray<Node> slots;
    slots.add(slot1Node);
    slots.add(slot2Node);
    slots.add(slot3Node);
    
    for (int i = 0; i < 3; ++i) {
        auto &choice = choices.getReference(i);
        auto slot = slots.getUnchecked(i);
        
        if (choice->getIndex() == 0) {
            if (slot != nullptr) {
                mainProcessor->removeNode(slot.get());
                slots.set(i, nullptr);
                hasChanged = true;
            }
        }
        else if (choice->getIndex() == 1) {
            if (slot != nullptr) {
                if (slot->getProcessor()->getName() == "Oscillator") {
                    continue;
                }
                mainProcessor->removeNode(slot.get());
            }
            slots.set(i, mainProcessor->addNode(std::make_unique<OscillatorProcessor>()));
            hasChanged = true;
        }
        else if (choice->getIndex() == 2) {
            if (slot != nullptr) {
                if (slot->getProcessor()->getName() == "Gain") {
                    continue;
                }
                mainProcessor->removeNode(slot.get());
            }
            slots.set(i, mainProcessor->addNode(std::make_unique<GainProcessor>()));
            hasChanged = true;
        }
        else if (choice->getIndex() == 3) {
            if (slot != nullptr) {
                if (slot->getProcessor()->getName() == "Filter") {
                    continue;
                }
                mainProcessor->removeNode(slot.get());
            }
            slots.set(i, mainProcessor->addNode(std::make_unique<FilterProcessor>()));
            hasChanged = true;
        }
    }
    
    if (hasChanged) {
        for (auto connection : mainProcessor->getConnections()) {
            mainProcessor->removeConnection(connection);
        }
        
        juce::ReferenceCountedArray<Node> activeSlots;
        
        for (auto slot : slots) {
            activeSlots.add(slot);
            slot->getProcessor()->setPlayConfigDetails(getMainBusNumInputChannels(), getMainBusNumOutputChannels(), getSampleRate(), getBlockSize());
        }
        
        if (activeSlots.isEmpty()) {
            connectAudioNodes();
        }
        else {
            for (int i = 0; i < activeSlots.size() - 1; ++i) {
                for (int channel = 0; channel < 2; ++channel) {
                    mainProcessor->addConnection({
                        { activeSlots.getUnchecked(i)->nodeID, channel },
                        { activeSlots.getUnchecked(i + 1)->nodeID, channel }
                    });
                }
                
                for (int channel = 0; channel < 2; ++channel) {
                    mainProcessor->addConnection({
                        { audioInputNode->nodeID, channel},
                        { activeSlots.getFirst()->nodeID, channel}
                    });
                    mainProcessor->addConnection({
                        { activeSlots.getLast()->nodeID, channel },
                        { audioOutputNode->nodeID, channel }
                    });
                }
            }
        }
        connectMidiNodes();
        
        for (auto node : mainProcessor->getNodes()) {
            node->getProcessor()->enableAllBuses();
        }
        
        for (int i = 0; i < 3; ++i) {
            auto slot = slots.getUnchecked(i);
            auto &bypass = bypasses.getReference(i);
            
            if (slot != nullptr) {
                slot->setBypassed(bypass->get());
            }
        }
        
        audioInputNode->setBypassed(muteInput->get());
        
        slot1Node = slots.getUnchecked(0);
        slot2Node = slots.getUnchecked(1);
        slot3Node = slots.getUnchecked(2);
    }
}

void AudioProcessorGraphTutorialAudioProcessor::connectAudioNodes()
{
    for (int channel = 0; channel < 2; ++channel) {
        mainProcessor->addConnection({ { audioInputNode->nodeID, channel},
                                        { audioOutputNode->nodeID, channel} });
    }
}

void AudioProcessorGraphTutorialAudioProcessor::connectMidiNodes()
{
    mainProcessor->addConnection({ { midiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex},
                                    { midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex} });
}

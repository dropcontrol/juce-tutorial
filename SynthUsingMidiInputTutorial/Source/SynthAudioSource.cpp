/*
  ==============================================================================

    SynthAudioSource.cpp
    Created: 17 Apr 2021 7:26:20pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthAudioSource.h"

//==============================================================================
SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState &keyState)
    :   keyboardState(keyState)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    for (auto i = 0; i < 4; ++i) {
        synth.addVoice(new SineWaveVoice());
    }
    
    synth.addSound(new SineWaveSound());
}

//SynthAudioSource::~SynthAudioSource()
//{
//}

void SynthAudioSource::prepareToPlay(int, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiCollector.reset(sampleRate);
}

void SynthAudioSource::releaseResources() {}

void SynthAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
    
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
}

void SynthAudioSource::setUsingSineWaveSound()
{
    synth.clearSounds();
}

juce::MidiMessageCollector *SynthAudioSource::getMidiCollector()
{
    return &midiCollector;
}

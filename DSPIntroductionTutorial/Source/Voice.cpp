/*
  ==============================================================================

    Voice.cpp
    Created: 23 Apr 2021 4:36:03pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "Voice.h"

Voice::Voice()
{
    auto &masterGain = processorChain.get<masterGainIndex>();
    masterGain.setGainLinear(0.7f);
}

void Voice::prepare(const juce::dsp::ProcessSpec &spec)
{
    tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
    processorChain.prepare(spec);
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    processorChain.get<osc1index>().setFrequency(freqHz, true);
    processorChain.get<osc1index>().setLevel(velocity);
}

void Voice::notePitchbendChanged()
{
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    processorChain.get<osc1index>().setFrequency(freqHz);
}

void Voice::noteStopped(bool)
{
    clearCurrentNote();
}

void Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    auto block = tempBlock.getSubBlock(0, (size_t)numSamples);
    block.clear();
    juce::dsp::ProcessContextReplacing<float> context(block);
    processorChain.process(context);
    
    juce::dsp::AudioBlock<float> (outputBuffer).getSubBlock((size_t)startSample, (size_t)numSamples).add(tempBlock);
}


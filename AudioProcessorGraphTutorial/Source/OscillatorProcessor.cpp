/*
  ==============================================================================

    OscillatorProcessor.cpp
    Created: 21 Apr 2021 1:42:44pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "OscillatorProcessor.h"

OscillatorProcessor::OscillatorProcessor()
{
    oscillator.setFrequency(440.0f);
    oscillator.initialise([] (float x){ return std::sin(x); });
}

void OscillatorProcessor::prepareToPlay(double sampleRate, int samplePerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(samplePerBlock) };
    oscillator.prepare(spec);
}

void OscillatorProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    oscillator.process(context);
}

void OscillatorProcessor::reset()
{
    oscillator.reset();
}

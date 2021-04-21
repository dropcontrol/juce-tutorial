/*
  ==============================================================================

    OscillatorProcessor.h
    Created: 21 Apr 2021 1:42:44pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProcessorBase.h"

class OscillatorProcessor   : public ProcessorBase
{
public:
    OscillatorProcessor();
    
    void prepareToPlay(double sampleRate, int samplePerBlock) override;
    void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &) override;
    void reset() override;
    
    const juce::String getName() const override { return "Oscillator"; }
private:
    juce::dsp::Oscillator<float> oscillator;
    
};

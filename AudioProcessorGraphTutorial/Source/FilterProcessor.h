/*
  ==============================================================================

    FilterProcessor.h
    Created: 21 Apr 2021 2:15:18pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProcessorBase.h"

//==============================================================================
/*
*/
class FilterProcessor  : public ProcessorBase
{
public:
    FilterProcessor();
//    ~FilterProcessor() override;
    
    void prepareToPlay(double sampleRate, int samplePerBlock) override;
    void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &) override;
    void reset() override;

    const juce::String getName() const override { return "Filter"; }

private:
    
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;
    
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor)
};

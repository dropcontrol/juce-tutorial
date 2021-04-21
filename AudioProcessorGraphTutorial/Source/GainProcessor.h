/*
  ==============================================================================

    GainProcessor.h
    Created: 21 Apr 2021 2:01:49pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ProcessorBase.h"

//==============================================================================
/*
*/
class GainProcessor  : public ProcessorBase
{
public:
    GainProcessor();
//    ~GainProcessor() override;
    
    void prepareToPlay(double sampleRate, int samplePerBlock) override;
    void processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &) override;
    void reset() override;

    const juce::String getName() const override { return "Gain"; }
    
private:
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessor)
    juce::dsp::Gain<float> gain;
};

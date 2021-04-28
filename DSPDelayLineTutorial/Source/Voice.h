/*
  ==============================================================================

    Voice.h
    Created: 23 Apr 2021 4:36:03pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomOscillator.h"

//==============================================================================
class Voice  : public juce::MPESynthesiserVoice
{
public:
    Voice();
    
    void prepare(const juce::dsp::ProcessSpec &spec);
    void noteStarted() override;
    void notePitchbendChanged() override;
    void noteStopped (bool) override;

    void notePressureChanged() override {}
    void noteTimbreChanged()   override {}
    void noteKeyStateChanged() override {}
    
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    enum {
        osc1index,
        /*osc2index,*/
        /*osc3index,*/
        /*filterIndex,*/
        masterGainIndex
    };
    
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    
    juce::dsp::ProcessorChain<CustomOscillator<float>, /*CustomOscillator<float>, juce::dsp::LadderFilter<float>,*/ juce::dsp::Gain<float>> processorChain;
    
    static constexpr size_t lfoDownsamplingRatio = 128;
    size_t lfoProcessingIndex = lfoDownsamplingRatio;
    juce::dsp::Oscillator<float> lfo;
};


/*
  ==============================================================================

    AudioEngine.h
    Created: 23 Apr 2021 4:30:45pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "Distortion.h"
#include "CabSimulator.h"
#include "Delay.h"

class AudioEngine   :   public juce::MPESynthesiser
{
public:
    static constexpr auto maxNumVoices = 4;
    
    AudioEngine();
    void prepare (const juce::dsp::ProcessSpec &spec) noexcept;

    
private:
    void renderNextSubBlock (juce::AudioBuffer<float> &outputAudio, int startSample, int numSamples) override;

    enum {
//        distortionIndex,
//        cabSimulatorIndex,
        delayIndex
//        reverbIndex
    };
    
    juce::dsp::ProcessorChain<Delay<float>> fxChain;
//    juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>, juce::dsp::Reverb> fxChain;
//    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
//    juce::dsp::ProcessorChain<Distortion<float>, juce::dsp::Reverb> fxChain;


};

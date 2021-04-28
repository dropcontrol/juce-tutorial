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

class AudioEngine   :   public juce::MPESynthesiser
{
public:
    static constexpr auto maxNumVoices = 4;
    
    AudioEngine();
    void prepare (const juce::dsp::ProcessSpec &spec) noexcept;

    
private:
    void renderNextSubBlock (juce::AudioBuffer<float> &outputAudio, int startSample, int numSamples) override;

    enum {
        distortionIndex,
        cabSimulatorIndex
//        reverbIndex
    };
    
    juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>> fxChain;
//    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
//    juce::dsp::ProcessorChain<Distortion<float>, juce::dsp::Reverb> fxChain;


};

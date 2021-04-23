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

class AudioEngine   :   public juce::MPESynthesiser
{
public:
    static constexpr auto maxNumVoices = 4;
    
    AudioEngine();
    void prepare (const juce::dsp::ProcessSpec &spec) noexcept;

    
private:
    void renderNextSubBlock (juce::AudioBuffer<float> &outputAudio, int startSample, int numSamples) override;

    enum {
        reverbIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
};

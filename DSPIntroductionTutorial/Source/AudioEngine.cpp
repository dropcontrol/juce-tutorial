/*
  ==============================================================================

    AudioEngine.cpp
    Created: 23 Apr 2021 4:30:45pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    for (auto i = 0; i < maxNumVoices; ++i) {
        addVoice(new Voice);
    }
    
    setVoiceStealingEnabled(true);
}

void AudioEngine::prepare(const juce::dsp::ProcessSpec &spec) noexcept
{
    setCurrentPlaybackSampleRate(spec.sampleRate);
    
    for (auto *v : voices) {
        dynamic_cast<Voice*>(v)->prepare(spec);
    }
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float> &outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);
}

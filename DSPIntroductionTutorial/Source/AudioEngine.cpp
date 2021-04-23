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
    
    fxChain.prepare(spec);
}

void AudioEngine::renderNextSubBlock(juce::AudioBuffer<float> &outputAudio, int startSample, int numSamples)
{
    MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);
    
    auto block = juce::dsp::AudioBlock<float> (outputAudio);
    auto blockToUse = block.getSubBlock((size_t)startSample, (size_t)numSamples);
    auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
    fxChain.process(contextToUse);
}

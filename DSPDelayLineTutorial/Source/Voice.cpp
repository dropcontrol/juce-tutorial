/*
  ==============================================================================

    Voice.cpp
    Created: 23 Apr 2021 4:36:03pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "Voice.h"

Voice::Voice()
{
    lfo.initialise([](float x){ return std::sin(x); }, 128);
    lfo.setFrequency(3.0f);
    
    auto waveform = CustomOscillator<float>::Waveform::saw;
    processorChain.get<osc1index>().setWaveForm(waveform);
//    processorChain.get<osc2index>().setWaveForm(waveform);
    
    auto &masterGain = processorChain.get<masterGainIndex>();
    masterGain.setGainLinear(0.7f);
    
//    auto &filter = processorChain.get<filterIndex>();
//    filter.setMode(juce::dsp::LadderFilter<float>::Mode::LPF24);
//    filter.setResonance(0.7f);
//    filter.setCutoffFrequencyHz(500.0f);

}

void Voice::prepare(const juce::dsp::ProcessSpec &spec)
{
    lfo.prepare({ spec.sampleRate / lfoDownsamplingRatio, spec.maximumBlockSize, 1 });
    tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
    processorChain.prepare(spec);
    
}

void Voice::noteStarted()
{
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    
    processorChain.get<osc1index>().setFrequency(freqHz, true);
//    processorChain.get<osc1index>().setLevel(velocity);
    
//    processorChain.get<osc2index>().setFrequency(freqHz * 1.01f, true);
//    processorChain.get<osc2index>().setLevel(velocity);
    
//    processorChain.get<osc3index>().setFrequency(freqHz * 0.99f, true);
//    processorChain.get<osc3index>().setLevel(velocity);
    
    auto &stringModel = processorChain.get<stringIndex>();
    stringModel.setFrequency(freqHz);
    stringModel.trigger(velocity);
}

void Voice::notePitchbendChanged()
{
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    processorChain.get<osc1index>().setFrequency(freqHz);
//    processorChain.get<osc2index>().setFrequency(freqHz * 1.01f);
//    processorChain.get<osc3index>().setFrequency(freqHz * 0.99f);
}

void Voice::noteStopped(bool)
{
    clearCurrentNote();
}

void Voice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    auto block = tempBlock.getSubBlock(0, (size_t)numSamples);
    block.clear();
    juce::dsp::ProcessContextReplacing<float> context(block);
    processorChain.process(context);
    
//    auto output = tempBlock.getSubBlock(0, (size_t)numSamples);
//    output.clear();
//
//    for (size_t pos = 0; pos < (size_t)numSamples;) {
//        auto max = juce::jmin((size_t)numSamples - pos, lfoUpdateCounter);
//        auto block = output.getSubBlock(pos, max);
//
//        juce::dsp::ProcessContextReplacing<float> context(block);
//        processorChain.process(context);
//
//        pos += max;
//        lfoUpdateCounter -= max;
//
//        if (lfoUpdateCounter == 0) {
//            lfoUpdateCounter = lfoUpdateRate;
//            auto lfoOut = lfo.processSample(0.0f);
//            auto curoffFreqHz = juce::jmap(lfoOut, -1.0f, 1.0f, 100.0f, 2000.0f);
//            processorChain.get<filterIndex>().setCutoffFrequencyHz(curoffFreqHz);
//        }
//    }
    
//    for (int i = 0; i < numSamples; ++i) {
//        if (--lfoProcessingIndex == 0) {
//            lfoProcessingIndex = lfoDownsamplingRatio;
//            auto lfoOut = lfo.processSample(0.0f);
//            auto cutoffHz = juce::jmap(lfoOut, -1.0f, 1.0f, 100.0f, 4e3f);
////            processorChain.get<filterIndex>().setCutoffFrequencyHz(cutoffHz);
//        }
//    }
//
//    auto block = tempBlock.getSubBlock(0, (size_t)numSamples);
//    block.clear();
//    juce::dsp::ProcessContextReplacing<float> context(block);
//    processorChain.process(context);
//
//    juce::dsp::AudioBlock<float> (outputBuffer).getSubBlock((size_t)startSample, (size_t)numSamples).add(tempBlock);
    
    // silence detector
    bool active = false;
    for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
    {
        auto* channelPtr = block.getChannelPointer (ch);

        for (int i = 0; i < numSamples; ++i)
        {
            if (channelPtr[i] != 0.0f)
            {
                active = true;
                break;
            }
        }
    }

    if (active)
    {
        juce::dsp::AudioBlock<float> (outputBuffer)
            .getSubBlock ((size_t) startSample, (size_t) numSamples)
            .add (tempBlock);
    }
    else
    {
        clearCurrentNote();
    }
}


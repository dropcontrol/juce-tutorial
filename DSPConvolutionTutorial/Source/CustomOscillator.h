/*
  ==============================================================================

    CustomOscillator.h
    Created: 23 Apr 2021 4:47:44pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename Type>
class CustomOscillator {
public:
    CustomOscillator() {
        setWaveForm(Waveform::sine);
        auto &gain = processorChain.template get<gainIndex>();
        gain.setRampDurationSeconds(3e-2);
        gain.setGainLinear(Type (0));
        
//        auto &osc = processorChain.template get<oscIndex>();
        
//        // サイン波オシレータ
////        osc.initialise([](Type x) { return std::sin(x);}, 128);
//
//        // ノコギリ波オシレータ
//        osc.initialise([](Type x) {
//            return juce::jmap(x,
//                              Type(-juce::MathConstants<double>::pi),
//                              Type(juce::MathConstants<double>::pi),
//                              Type(-1),
//                              Type(1));
//        }, 2);
    };
    
    enum class Waveform {
        sine,
        saw
    };
    
    void setWaveForm(Waveform waveform)
    {
        switch(waveform)
        {
            case Waveform::sine:
                processorChain.template get<oscIndex>().initialise([](Type x){
                    return std::sin(x);
                }, 128);
                break;
            case Waveform::saw:
                processorChain.template get<oscIndex>().initialise([](Type x){
                    return juce::jmap(x, Type(-juce::double_Pi), Type(juce::double_Pi), Type(-1), Type(1));
                }, 128);
                break;
            default:
                jassertfalse;
                break;
        }
    }
    
    void setFrequency(Type newValue, bool force = false)
    {
//        auto &osc = processorChain.template get<oscIndex>();
//        osc.setFrequency(newValue, force);
        processorChain.template get<oscIndex>().setFrequency(newValue, force);
    }
    
    void setLevel(Type newValue)
    {
//        auto &gain = processorChain.template get<gainIndex>();
//        gain.setGainLinear(newValue);
        processorChain.template get<gainIndex>().setGainLinear(newValue);
    }
    
    void reset () noexcept
    {
        processorChain.reset();
    }
    
    template <typename ProcessContext>
    void process(const ProcessContext &context) noexcept
    {
        auto&& outBlock = context.getOutputBlock();
        auto blockToUse = tempBlock.getSubBlock(0, outBlock.getNumSamples());
        juce::dsp::ProcessContextReplacing<float> tempContext(blockToUse);
        processorChain.process(context);
        
        outBlock.copyFrom(context.getInputBlock()).add(blockToUse);
    }
    
    void prepare(const juce::dsp::ProcessSpec &spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare(spec);
    }
    
private:
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;
    
    enum
    {
        oscIndex,
        gainIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
};

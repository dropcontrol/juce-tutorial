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
        auto &osc = processorChain.template get<oscIndex>();
        osc.initialise([](Type x) { return std::sin(x);}, 128);
    };
    
    void setFrequency(Type newValue, bool force = false)
    {
        auto &osc = processorChain.template get<oscIndex>();
        osc.setFrequency(newValue, force);
    }
    
    void setLevel(Type newValue)
    {
        auto &gain = processorChain.template get<gainIndex>();
        gain.setGainLinear(newValue);
    }
    
    void reset () noexcept
    {
        processorChain.reset();
    }
    
    template <typename ProcessContext>
    void process(const ProcessContext &context) noexcept
    {
        processorChain.process(context);
    }
    
    void prepare(const juce::dsp::ProcessSpec &spec)
    {
        processorChain.prepare(spec);
    }
    
private:
    enum
    {
        oscIndex,
        gainIndex
    };
    
    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
};

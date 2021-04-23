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
    CustomOscillator() {};
    
    void setFrequency(Type newValue, bool force = false)
    {
        juce::ignoreUnused(newValue, force);
    }
    
    void setLevel(Type newValue)
    {
        juce::ignoreUnused(newValue);
    }
    
    void reset () noexcept {}
    
    template <typename ProcessContext>
    void process(const ProcessContext &context) noexcept
    {
        juce::ignoreUnused(context);
    }
    
    void prepare(const juce::dsp::ProcessSpec &spec)
    {
        juce::ignoreUnused(spec);
    }
    
private:
};

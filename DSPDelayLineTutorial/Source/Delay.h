/*
  ==============================================================================

    Delay.h
    Created: 28 Apr 2021 1:34:13pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DelayLine.h"

template <typename Type, size_t maxNumChannels = 2>
class Delay {
public:
    Delay()
    {
        setMaxDelayTime(2.0f);
        setDelayTime(0, 0.7f);
        setDelayTime(1, 0.5f);
        setWetLevel(0.8f);
        setFeedback(0.5f);
    }
    
    void prepare(const juce::dsp::ProcessSpec &spec)
    {
        jassert(spec.numChannels <= maxNumChannels);
        sampleRate = (Type)spec.sampleRate;
        updateDelayLineSize();
        updateDelayTime();
        
//        filterCoefs = juce::dsp::IIR::Coefficients<Type>::makeFirstOrderLowPass(sampleRate, Type(1e3));
        filterCoefs = juce::dsp::IIR::Coefficients<Type>::makeFirstOrderHighPass (sampleRate, Type (1e3));

        
        for (auto &f : filters) {
            f.prepare(spec);
            f.coefficients = filterCoefs;
        }
    }
    
    void reset() noexcept
    {
        for (auto &f : filters) {
            f.reset();
        }
        
        for (auto &dline : delayLines) {
            dline.clear();
        }
    }
    
    size_t getNumChannels() const noexcept
    {
        return delayLines.size();
    }
    
    void setMaxDelayTime(Type newValue)
    {
        jassert(newValue > Type(0));
        maxDelayTime = newValue;
        updateDelayLineSize();
    }
    
    void setFeedback(Type newValue) noexcept
    {
        jassert(newValue >= Type(0) && newValue <= Type(1));
        feedback = newValue;
    }
    
    void setWetLevel(Type newValue) noexcept
    {
        jassert(newValue >= Type(0) && newValue <= Type(1));
        wetLevel = newValue;
    }
    
    void setDelayTime(size_t channel, Type newValue)
    {
        if (channel >= getNumChannels()) {
            jassertfalse;
            return;
        }
        
        jassert(newValue >= Type(0));
        delayTimes[channel] = newValue;
        
        updateDelayTime();
    }

    template <typename ProcessContext>
    void process(const ProcessContext &context) noexcept
    {
        auto &inputBlock = context.getInputBlock();
        auto &outputBlock = context.getOutputBlock();
        auto numSamples = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();
        
        jassert(inputBlock.getNumSamples() == numSamples);
        jassert(inputBlock.getNumChannels() == numChannels);
        
        for (size_t ch = 0; ch < numChannels; ++ch) {
            auto *input = inputBlock.getChannelPointer(ch);
            auto *output = outputBlock.getChannelPointer(ch);
            auto &dline = delayLines[ch];
            auto delayTime = delayTimeSample[ch];
            auto &filter = filters[ch];
            
            for (size_t i = 0; i < numSamples; ++i) {
//                auto delayedSample = dline.get(delayTime);
                auto delayedSample = filter.processSample (dline.get (delayTime));
                auto inputSample = input[i];
                auto dlineInputSample = std::tanh(inputSample + feedback + delayedSample);
                dline.push(dlineInputSample);
                auto outputSample = inputSample + wetLevel * delayedSample;
                output[i] = outputSample;
            }
        }
    }
    
    
private:
    std::array<DelayLine<Type>, maxNumChannels> delayLines;
    std::array<size_t, maxNumChannels> delayTimeSample;
    std::array<Type, maxNumChannels> delayTimes;
    
    Type feedback { Type(0) };
    Type wetLevel { Type(0) };
    
    Type maxDelayTime { Type(2) };
    Type sampleRate { Type(44.1e3) };
    
    std::array<juce::dsp::IIR::Filter<Type>, maxNumChannels> filters;
    typename juce::dsp::IIR::Coefficients<Type>::Ptr filterCoefs;
    
    void updateDelayLineSize()
    {
        auto delayLineSizeSamples = (size_t)std::ceil(maxDelayTime * sampleRate);
        for (auto &dline : delayLines) {
            dline.resize(delayLineSizeSamples);
        }
    }
    
    void updateDelayTime() noexcept
    {
        for (size_t ch = 0; ch < maxNumChannels; ++ch) {
            delayTimeSample[ch] = (size_t)juce::roundToInt(delayTimes[ch] * sampleRate);
        }
    }
};

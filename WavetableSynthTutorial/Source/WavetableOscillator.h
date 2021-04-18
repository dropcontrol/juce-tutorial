/*
  ==============================================================================

    WavetableOscillator.h
    Created: 18 Apr 2021 4:39:57pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class WavetableOscillator
{
public:
    WavetableOscillator(const juce::AudioSampleBuffer &wavetableToUse);
    
    void setFrequency(float frequency, float sampleRate);
    
    forcedinline float getNextSample() noexcept
    {
//        auto tableSize = (unsigned int)wavetable.getNumSamples();
        
        auto index0 = (unsigned int)currentIndex;
//        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
        auto index1 = index0 + 1;
        
        auto frac = currentIndex - (float)index0;
        
        auto *table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        
        auto currentSample = value0 + frac * (value1 - value0);
        
        if ((currentIndex += tableDelta) > (float)tableSize) {
            currentIndex -= (float)tableSize;
        }
        return currentSample;
    }
    
private:
    const juce::AudioSampleBuffer &wavetable;
    const int tableSize;
    float currentIndex = 0.0f;
    float tableDelta = 0.0f;
};



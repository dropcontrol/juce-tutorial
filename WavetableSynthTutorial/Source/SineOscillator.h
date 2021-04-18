/*
  ==============================================================================

    SineOscillator.h
    Created: 18 Apr 2021 2:20:22pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class SineOscillator
{
public:
    SineOscillator();
    
    void setFrequency(float frequency, float sampleRate);
    
    forcedinline float getNextSample() noexcept
    {
        auto currentSample = std::sin(currentAngle);
        updateAngle();
        return currentSample;
    }

    forcedinline void updateAngle() noexcept
    {
        currentAngle += angleDelta;
        if (currentAngle >= juce::MathConstants<float>::twoPi) {
            currentAngle -= juce::MathConstants<float>::twoPi;
        }
    }
     
private:
    float currentAngle = 0.0f;
    float angleDelta = 0.0f;
    
};

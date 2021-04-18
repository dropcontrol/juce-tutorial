/*
  ==============================================================================

    SineOscillator.cpp
    Created: 18 Apr 2021 2:20:22pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "SineOscillator.h"

//==============================================================================
SineOscillator::SineOscillator() {}

void SineOscillator::setFrequency(float frequency, float sampleRate)
{
    auto cyclePerSample = frequency / sampleRate;
    angleDelta = cyclePerSample * juce::MathConstants<float>::twoPi;
}


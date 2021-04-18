/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 18 Apr 2021 4:39:57pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator(const juce::AudioSampleBuffer &wavetableToUse)
    : wavetable(wavetableToUse),
      tableSize(wavetable.getNumSamples() - 1)
{
    jassert(wavetable.getNumChannels() == 1);
}

void WavetableOscillator::setFrequency(float frequency, float sampleRate)
{
    auto tableSizeOverSampleRate = (float)tableSize / sampleRate;
    tableDelta = frequency * tableSizeOverSampleRate;}



/*
  ==============================================================================

    MPEDemoSynthVoice.cpp
    Created: 19 Apr 2021 2:14:08pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include "MPEDemoSynthVoice.h"

MPEDemoSynthVoice::MPEDemoSynthVoice() {}

void MPEDemoSynthVoice::noteStarted()
{
    jassert(currentlyPlayingNote.isValid());
    jassert(currentlyPlayingNote.keyState == juce::MPENote::keyDown
            || currentlyPlayingNote.keyState == juce::MPENote::keyDownAndSustained);
    
    // get data from the current MPENote
    level.setTargetValue(currentlyPlayingNote.pressure.asUnsignedFloat());
    frequency.setTargetValue(currentlyPlayingNote.getFrequencyInHertz());
    timbre.setTargetValue(currentlyPlayingNote.timbre.asUnsignedFloat());
    
    phase = 0.0;
    auto cyclesPerSample = frequency.getNextValue() / currentSampleRate;
    phaseDelta = 2.0 * juce::MathConstants<double>::pi *cyclesPerSample;
    
    tailOff = 0.0;
}

void MPEDemoSynthVoice::noteStopped(bool allowTailOff)
{
    jassert(currentlyPlayingNote.keyState == juce::MPENote::off);
    
    if (allowTailOff) {
        if (tailOff == 0.0) {
            tailOff = 1.0;
        }
    }
    else {
        clearCurrentNote();
        phaseDelta = 0.0;
    }
}

void MPEDemoSynthVoice::notePressureChanged()
{
    level.setTargetValue(currentlyPlayingNote.pressure.asUnsignedFloat());
}

void MPEDemoSynthVoice::notePitchbendChanged()
{
    frequency.setTargetValue(currentlyPlayingNote.getFrequencyInHertz());
}

void MPEDemoSynthVoice::noteTimbreChanged()
{
    timbre.setTargetValue(currentlyPlayingNote.timbre.asUnsignedFloat());
}

void MPEDemoSynthVoice::noteKeyStateChanged() {}

void MPEDemoSynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSample)
{
    if (phaseDelta != 0.0) {
        if (tailOff > 0.0) {
            while (--numSample >= 0) {
                auto currentSample = getNextSample() * (float)tailOff;
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;) {
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                
                ++startSample;
                tailOff *= 0.99;
                
                if (tailOff <= 0.005) {
                    clearCurrentNote();
                    phaseDelta = 0.0;
                    break;;
                }
            }
        }
        else {
            while (--numSample >= 0) {
                auto currentSample = getNextSample();
                
                for (auto i = outputBuffer.getNumChannels(); --i >=0;) {
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                
                ++startSample;
            }
        }
    }
}

float MPEDemoSynthVoice::getNextSample() noexcept
{
    auto levelDb = (level.getNextValue() - 1.0) * maxLevelDb;
    auto amplitude = std::pow(10.0f, 0.05f * levelDb) * maxLevel;
    
    auto f1 = std::sin(phase);
    auto f2 = std::copysign(1.0, f1);
    auto a2 = timbre.getNextValue();
    auto a1 = 1.0 - a2;
    
    auto nextSample = float(amplitude * ((a1 * f1) + (a2 * f2)));
    
    auto cyclesPerSample = frequency.getNextValue() / currentSampleRate;
    phaseDelta = 2.0 * juce::MathConstants<double>::pi * cyclesPerSample;
    phase = std::fmod(phase + phaseDelta, 2.0 * juce::MathConstants<double>::pi);
    
    return nextSample;
}

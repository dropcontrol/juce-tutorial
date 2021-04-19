/*
  ==============================================================================

    MPEDemoSynthVoice.h
    Created: 19 Apr 2021 2:14:08pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MPEDemoSynthVoice : public juce::MPESynthesiserVoice
{
public:
    MPEDemoSynthVoice();

    void noteStarted() override;
    void noteStopped(bool allowTailOff) override;
    void notePressureChanged() override;
    void notePitchbendChanged() override;
    void noteTimbreChanged() override;
    void noteKeyStateChanged() override;
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSample) override;
    

private:
    float getNextSample() noexcept;
    
    juce::SmoothedValue<double> level, timbre, frequency;

    double phase = 0.0;
    double phaseDelta = 0.0;
    double tailOff = 0.0;

    static constexpr auto maxLevel = 0.05;
    static constexpr auto maxLevelDb = 31.0;
    static constexpr auto smothingLengthInSeconds = 0.01;
};

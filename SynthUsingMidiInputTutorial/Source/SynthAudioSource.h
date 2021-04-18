/*
  ==============================================================================

    SynthAudioSource.h
    Created: 17 Apr 2021 7:26:20pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
struct SineWaveSound    : public juce::SynthesiserSound
{
    SineWaveSound() {}
    
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};

//==============================================================================
struct SineWaveVoice   : public juce::SynthesiserVoice
{
    SineWaveVoice() {}
    
    bool canPlaySound(juce::SynthesiserSound *sound) override
    {
        return dynamic_cast<juce::SynthesiserSound *>(sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        tailOn = 0.0;
        
        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff) {
            if (tailOff == 0.0) tailOff = 1.0;
            if (tailOn == 1.0) tailOn = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            if (tailOn < 1.0) {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float)(std::sin(currentAngle) * level * tailOn);
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOn += 0.0001;
                    
                }
            }
            else if (tailOff > 0.0)
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float)(std::sin(currentAngle) * level * tailOff);
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOff *= 0.9999;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float)(std::sin(currentAngle) * level);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);
                    
                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }
    

private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0, tailOn = 0.0;
};

//==============================================================================
/*
*/
class SynthAudioSource  : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState &keyState);
//    ~SynthAudioSource() override;
    
    void prepareToPlay(int /*samplePerBlockExpected*/, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    
    void setUsingSineWaveSound();
    
    juce::MidiMessageCollector *getMidiCollector();

private:
    juce::MidiKeyboardState &keyboardState;
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthAudioSource)
};

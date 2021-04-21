/*
  ==============================================================================

    GainProcessor.cpp
    Created: 21 Apr 2021 2:01:49pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainProcessor.h"

//==============================================================================
GainProcessor::GainProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    gain.setGainDecibels(-6.0f);

}

//GainProcessor::~GainProcessor()
//{
//}

void GainProcessor::prepareToPlay(double sampleRate, int samplePerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32>(samplePerBlock), 2};
    gain.prepare(spec);
}

void GainProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    gain.process(context);
}

void GainProcessor::reset()
{
    gain.reset();
}

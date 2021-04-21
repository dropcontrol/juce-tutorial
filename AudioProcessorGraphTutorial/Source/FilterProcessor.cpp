/*
  ==============================================================================

    FilterProcessor.cpp
    Created: 21 Apr 2021 2:15:18pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterProcessor.h"

//==============================================================================
FilterProcessor::FilterProcessor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

//FilterProcessor::~FilterProcessor()
//{
//}

void FilterProcessor::prepareToPlay(double sampleRate, int samplePerBlock)
{
    *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 1000.0f);
    
    juce::dsp::ProcessSpec spec {sampleRate, static_cast<juce::uint32>(samplePerBlock), 2};
    filter.prepare(spec);
    
}

void FilterProcessor::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);
}

void FilterProcessor::reset()
{
    filter.reset();
}

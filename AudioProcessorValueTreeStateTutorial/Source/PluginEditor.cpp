/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioProcessorValueTreeStateTutorialAudioProcessorEditor::AudioProcessorValueTreeStateTutorialAudioProcessorEditor (AudioProcessorValueTreeStateTutorialAudioProcessor& p, juce::AudioProcessorValueTreeState &vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    gainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(gainLabel);
    
    addAndMakeVisible(gainSlider);
    gainAttachment.reset(new SliderAtachment(valueTreeState, "gain", gainSlider));
    
    invertButton.setButtonText("Invert Phase");
    addAndMakeVisible(invertButton);
    invertAttachment.reset(new ButtonAttachment(valueTreeState, "invertPhase", invertButton));
    
    setSize (paramSliderWidth + paramLabelWidth, juce::jmax(100, paramControlHeight * 2));
}

AudioProcessorValueTreeStateTutorialAudioProcessorEditor::~AudioProcessorValueTreeStateTutorialAudioProcessorEditor()
{
}

//==============================================================================
void AudioProcessorValueTreeStateTutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioProcessorValueTreeStateTutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto r = getLocalBounds();
    
    auto gainRect = r.removeFromTop(paramControlHeight);
    gainLabel.setBounds(gainRect.removeFromLeft(paramLabelWidth));
    gainSlider.setBounds(gainRect);
    
    invertButton.setBounds(r.removeFromTop(paramControlHeight));
}

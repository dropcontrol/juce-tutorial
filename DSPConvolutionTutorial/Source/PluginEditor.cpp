/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DSPIntroductionTutorialAudioProcessorEditor::DSPIntroductionTutorialAudioProcessorEditor (DSPIntroductionTutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), scopeComponet(audioProcessor.getAudioBufferQueue())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible (midiKeyboardComponent);
    addAndMakeVisible (scopeComponet);

    setSize (400, 300);

    auto area = getLocalBounds();
    scopeComponet.setTopLeftPosition (0, 80);
    scopeComponet.setSize (area.getWidth(), area.getHeight() - 100);

    midiKeyboardComponent.setMidiChannel (2);
    midiKeyboardState.addListener (&audioProcessor.getMidiMessageCollector());}

DSPIntroductionTutorialAudioProcessorEditor::~DSPIntroductionTutorialAudioProcessorEditor()
{
    midiKeyboardState.removeListener (&audioProcessor.getMidiMessageCollector());
}

//==============================================================================
void DSPIntroductionTutorialAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void DSPIntroductionTutorialAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    midiKeyboardComponent.setBounds (area.removeFromTop (80).reduced (8));

}

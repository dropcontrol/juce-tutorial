/*
  ==============================================================================

    VisualiserComponent.cpp
    Created: 19 Apr 2021 2:07:27pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VisualiserComponent.h"

//==============================================================================
VisualiserComponent::VisualiserComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

//VisualiserComponent::~VisualiserComponent()
//{
//}

void VisualiserComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colours::black);
    
    auto noteDistance = float (getWidth()) / 128;
    
    for (auto i = 0; i < 128; ++i) {
        auto x = noteDistance * (float) i;
        auto noteHeight = int(juce::MidiMessage::isMidiNoteBlack(i) ? 0.7 * getHeight() : getHeight());
        g.setColour(juce::MidiMessage::isMidiNoteBlack(i) ? juce::Colours::white : juce::Colours::grey);
        g.drawLine(x, 0.0f, x, (float)noteHeight);
        
        if (i > 0 && i % 12 == 0) {
            g.setColour(juce::Colours::grey);
            auto octaveNumber = (i / 12) - 2;
            g.drawText("C" + juce::String(octaveNumber), (int)x - 15, getWidth() - 30, 30, 30, juce::Justification::centredBottom);
        }
    }
}

//void VisualiserComponent::resized()
//{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//
//}

void VisualiserComponent::noteAdded(juce::MPENote newNote)
{
    const juce::ScopedLock sl (lock);
    activeNotes.add(newNote);
    triggerAsyncUpdate();
}

void VisualiserComponent::noteChanged(juce::MPENote changeNote)
{
    const juce::ScopedLock sl (lock);
    
    for (auto &note : activeNotes) {
        if (note.noteID == changeNote.noteID) {
            note = changeNote;
        }
    }
    triggerAsyncUpdate();
}

void VisualiserComponent::noteReleased(juce::MPENote finishedNote)
{
    const juce::ScopedLock sl (lock);
    
    for (auto i = activeNotes.size(); --i >= 0;) {
        if (activeNotes.getReference(i).noteID == finishedNote.noteID) {
            activeNotes.remove(i);
        }
    }
    triggerAsyncUpdate();
}

const juce::MPENote *VisualiserComponent::findActiveNote(int noteID) const noexcept
{
    for (auto &note : activeNotes) {
        if (note.noteID == noteID) {
            return &note;
        }
    }
    
    return nullptr;
}

NoteComponent* VisualiserComponent::findNoteComponent(int noteID) const noexcept
{
    for (auto &noteComp : noteComponents) {
        if (noteComp->note.noteID == noteID) {
            return noteComp;
        }
    }
    
    return nullptr;
}

void VisualiserComponent::handleAsyncUpdate()
{
    const juce::ScopedLock sl (lock);
    
    for (auto i = noteComponents.size(); --i >= 0;)
        if (findActiveNote (noteComponents.getUnchecked(i)->note.noteID) == nullptr)
            noteComponents.remove (i);

    for (auto& note : activeNotes)
        if (findNoteComponent (note.noteID) == nullptr)
            addAndMakeVisible (noteComponents.add (new NoteComponent (note)));

    for (auto& noteComp : noteComponents)
        if (auto* noteInfo = findActiveNote (noteComp->note.noteID))
            noteComp->update (*noteInfo, getCentrePositionForNote (*noteInfo));
}

juce::Point<float> VisualiserComponent::getCentrePositionForNote(juce::MPENote note) const
{
    auto n = float (note.initialNote) + float (note.totalPitchbendInSemitones);
    auto x = (float) getWidth() * n / 128;
    auto y = (float) getHeight() * (1 - note.timbre.asUnsignedFloat());

    return { x, y };
}

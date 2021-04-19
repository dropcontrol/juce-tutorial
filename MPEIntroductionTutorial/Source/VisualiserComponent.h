/*
  ==============================================================================

    VisualiserComponent.h
    Created: 19 Apr 2021 2:07:27pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteComponent.h"

//==============================================================================
/*
*/
class VisualiserComponent  : public juce::Component, public juce::MPEInstrument::Listener, private juce::AsyncUpdater
{
public:
    VisualiserComponent();
//    ~VisualiserComponent() override;

    void paint (juce::Graphics&) override;
//    void resized() override;
    
    void noteAdded(juce::MPENote newNote) override;
    
    void notePressureChanged  (juce::MPENote note) override { noteChanged (note); }
    void notePitchbendChanged (juce::MPENote note) override { noteChanged (note); }
    void noteTimbreChanged    (juce::MPENote note) override { noteChanged (note); }
    void noteKeyStateChanged  (juce::MPENote note) override { noteChanged (note); }
    
    void noteReleased(juce::MPENote finishedNote) override;
    void noteChanged(juce::MPENote changeNote);
    

private:
    const juce::MPENote *findActiveNote(int noteID) const noexcept;
    NoteComponent *findNoteComponent(int noteID) const noexcept;
    void handleAsyncUpdate() override;
    juce::Point<float> getCentrePositionForNote(juce::MPENote note) const;
    
    //==============================================================================
    juce::OwnedArray<NoteComponent> noteComponents;
    juce::CriticalSection lock;
    juce::Array<juce::MPENote> activeNotes;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualiserComponent)
};

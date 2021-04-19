/*
  ==============================================================================

    NoteComponent.h
    Created: 19 Apr 2021 3:56:19pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NoteComponent  : public juce::Component
{
public:
    NoteComponent(const juce::MPENote &n);
    ~NoteComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void update(const juce::MPENote &newNote, juce::Point<float> newCentre);
    
    //==============================================================================
    juce::MPENote note;
    juce::Colour colour;
    juce::Point<float> centre;

private:
    void drawPressedNoteCircle(juce::Graphics &g, juce::Colour zoneColour);
    void drawSustainedNoteCircle(juce::Graphics &g, juce::Colour zoneColour);
    void drawNoteLabel(juce::Graphics &g, juce::Colour);
    
    juce::Rectangle<float> getSquareAroundCentre(float radius) const noexcept;
    juce::Rectangle<float> translateToLocalBounds(juce::Rectangle<float> r) const noexcept;
    juce::Rectangle<float> getTextRectangle() const noexcept;
    
    float getNoteOnRadius() const noexcept;
    float getNoteOffRadius() const noexcept;
    float getPressureRadius() const noexcept;
    
    static constexpr auto maxNoteRadius = 100.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteComponent)
};

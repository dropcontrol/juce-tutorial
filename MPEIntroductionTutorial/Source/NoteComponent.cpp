/*
  ==============================================================================

    NoteComponent.cpp
    Created: 19 Apr 2021 3:56:19pm
    Author:  hiroshi yamato

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NoteComponent.h"

//==============================================================================
NoteComponent::NoteComponent(const juce::MPENote &n)
    : note(n), colour(juce::Colours::white)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

NoteComponent::~NoteComponent()
{
}

void NoteComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    if (note.keyState == juce::MPENote::keyDown || note.keyState == juce::MPENote::keyDownAndSustained) {
        drawPressedNoteCircle(g, colour);
    } else if (note.keyState == juce::MPENote::sustained) {
        drawSustainedNoteCircle(g, colour);
    } else {
        return;
    }
    
    drawNoteLabel(g, colour);
}

void NoteComponent::update(const juce::MPENote &newNote, juce::Point<float> newCentre)
{
    note = newNote;
    centre = newCentre;
    
    setBounds(getSquareAroundCentre(juce::jmax(getNoteOnRadius(), getNoteOffRadius(), getPressureRadius()))
              .getUnion(getTextRectangle())
              .getSmallestIntegerContainer()
              .expanded(3));
    repaint();
}

void NoteComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void NoteComponent::drawPressedNoteCircle(juce::Graphics &g, juce::Colour zoneColor)
{
    g.setColour(zoneColor.withAlpha(0.3f));
    g.fillEllipse(translateToLocalBounds(getSquareAroundCentre(getNoteOnRadius())));
    g.setColour(zoneColor);
    g.drawEllipse(translateToLocalBounds(getSquareAroundCentre(getPressureRadius())), 2.0f);
}

void NoteComponent::drawSustainedNoteCircle(juce::Graphics &g, juce::Colour zoneColour)
{
    g.setColour (zoneColour);
    juce::Path circle, dashedCircle;
    circle.addEllipse (translateToLocalBounds (getSquareAroundCentre (getNoteOffRadius())));
    const float dashLengths[] = { 3.0f, 3.0f };
    juce::PathStrokeType (2.0, juce::PathStrokeType::mitered).createDashedStroke (dashedCircle, circle, dashLengths, 2);
    g.fillPath (dashedCircle);
}

void NoteComponent::drawNoteLabel(juce::Graphics &g, juce::Colour)
{
    auto textBounds = translateToLocalBounds (getTextRectangle()).getSmallestIntegerContainer();
    g.drawText ("+", textBounds, juce::Justification::centred);
    g.drawText (juce::MidiMessage::getMidiNoteName (note.initialNote, true, true, 3), textBounds, juce::Justification::centredBottom);
    g.setFont (juce::Font (22.0f, juce::Font::bold));
    g.drawText (juce::String (note.midiChannel), textBounds, juce::Justification::centredTop);
}

juce::Rectangle<float> NoteComponent::getSquareAroundCentre(float radius) const noexcept
{
    return juce::Rectangle<float> (radius * 2.0f, radius * 2.0f).withCentre (centre);
}

juce::Rectangle<float> NoteComponent::translateToLocalBounds (juce::Rectangle<float> r) const noexcept
{
    return r - getPosition().toFloat();
}

juce::Rectangle<float> NoteComponent::getTextRectangle() const noexcept
{
    return juce::Rectangle<float> (30.0f, 50.0f).withCentre (centre);
}

float NoteComponent::getNoteOnRadius()   const noexcept
{
    return note.noteOnVelocity.asUnsignedFloat() * maxNoteRadius;
}

float NoteComponent::getNoteOffRadius()  const noexcept
{
    return note.noteOffVelocity.asUnsignedFloat() * maxNoteRadius;
}

float NoteComponent::getPressureRadius() const noexcept
{
    return note.pressure.asUnsignedFloat() * maxNoteRadius;
}

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    static juce::String getMidiMessageDescription (const juce::MidiMessage& m)
    {
        if (m.isNoteOn())
            return "Note on " + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
//        if (m.isNoteOn())
//            return "Note on velocity" + juce::String (m.getVelocity());
        if (m.isNoteOff())
            return "Note off " + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())
            return "Program change " + juce::String (m.getProgramChangeNumber());
        if (m.isPitchWheel())
            return "Pitch wheel " + juce::String (m.getPitchWheelValue());
        if (m.isAftertouch())
            return "After touch " + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + juce::String (m.getAfterTouchValue());
        if (m.isChannelPressure())
            return "Channel pressure " + juce::String (m.getChannelPressureValue());
        if (m.isAllNotesOff()) return "All notes off";
        if (m.isAllSoundOff()) return "All sound off";
        if (m.isMetaEvent()) return "Meta event";

        if (m.isController())
        {
            juce::String name (juce::MidiMessage::getControllerName (m.getControllerNumber()));

            if (name.isEmpty())
                name = "[" + juce::String (m.getControllerNumber()) + "]";

            return "Controller " + name + ": " + juce::String (m.getControllerValue());
        }

        return juce::String::toHexString (m.getRawData(), m.getRawDataSize());
    }

    void setNoteNumber(int noteNumber);
    void addMessageToList(const juce::MidiMessage &message);
    void logMessage(const juce::String &m);
    void addMessageToBuffer(const juce::MidiMessage &message);
    void timerCallback() override;
    
    juce::TextButton bassDrumButton;
    juce::TextButton snareDrumButton;
    juce::TextButton closedHiHatButton;
    juce::TextButton openHiHatButton;
    
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    
    juce::TextEditor midiMessagesBox;
    
    juce::MidiBuffer midiBuffer;
    double sampleRate = 44100.0;
    int previousSampleNumber = 0;
    
//    int midiChannel = 10;
    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

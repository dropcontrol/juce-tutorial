#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  :  public juce::AudioAppComponent,
                        private juce::MidiInputCallback,
                        private juce::MidiKeyboardStateListener
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
    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback : public juce::CallbackMessage
    {
    public:
        IncomingMessageCallback(MainComponent *o, const juce::MidiMessage &m, const juce::String &s)
            : owner(o), message(m), source(s)
        {}
        
        void messageCallback() override
        {
            if (owner != nullptr) {
                owner->addMessageToList(message, source);
            }
        }
        
        Component::SafePointer<MainComponent> owner;
        juce::MidiMessage message;
        juce::String source;
    };
    
    //==============================================================================
    // Your private member variables go here...
    static juce::String getMidiMessageDescription(const juce::MidiMessage &m)
    {
        if (m.isNoteOn())
            return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff())
            return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange())
            return "Program change " + juce::String(m.getProgramChangeNumber());
        if (m.isPitchWheel())
            return "Pitch wheel " + juce::String(m.getPitchWheelValue());
        if (m.isAftertouch())
            return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isChannelPressure())
            return "Channle pressure " + juce::String(m.getChannelPressureValue());
        if (m.isAllNotesOff())
            return "All notes off";
        if (m.isAllSoundOff())
            return "All sound off";
        if (m.isMetaEvent())
            return "Meta event";
        
        if (m.isController()) {
            juce::String name(juce::MidiMessage::getControllerName(m.getControllerNumber()));
            
            if (name.isEmpty())
                name = "[" + juce::String(m.getControllerNumber()) + "]";
            
            return "Controller" + name + ": " + juce::String(m.getControllerValue());
        }
        
        return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
    }
    
    void setMidiInput(int index);
    void handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) override;
    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    void postMessageToList(const juce::MidiMessage &message, const juce::String &source);
    void addMessageToList(const juce::MidiMessage &message, const juce::String &source);
    void logMessage(const juce::String &m);
    
    
    juce::AudioDeviceManager deviceManager;
    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;
    bool isAddingFromMidiInput = false;
    
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    
    juce::TextEditor midiMessageBox;
    double startTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

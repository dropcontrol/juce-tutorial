#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    :   keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
        startTime(juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 400);
    
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    
    juce::StringArray midiInputNames;
    
    for (auto input : midiInputs) {
        midiInputNames.add(input.name);
    }
    
    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };
    
    for (auto input : midiInputs) {
        if (deviceManager.isMidiInputDeviceEnabled(input.identifier)) {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }
    
    if (midiInputList.getSelectedId() == 0) {
        setMidiInput(0);
    }
    
    addAndMakeVisible(keyboardComponent);
    keyboardState.addListener(this);
    
    addAndMakeVisible(midiMessageBox);
    midiMessageBox.setMultiLine(true);
    midiMessageBox.setReturnKeyStartsNewLine(true);
    midiMessageBox.setReadOnly(true);
    midiMessageBox.setScrollbarsShown(true);
    midiMessageBox.setCaretVisible(true);
    midiMessageBox.setPopupMenuEnabled(true);
    midiMessageBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x32ffffff));
    midiMessageBox.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    midiMessageBox.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));
    
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto area = getLocalBounds();
    
    midiInputList.setBounds(area.removeFromTop(36).removeFromRight(getWidth() - 150).reduced(8));
    keyboardComponent.setBounds(area.removeFromTop(80).reduced(8));
    midiMessageBox.setBounds(area.reduced(8));
    
}

void MainComponent::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();
    
    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);
    
    auto newInput = list[index];
    
    if (! deviceManager.isMidiInputDeviceEnabled(newInput.identifier)) {
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);
    }
    
    deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);
    
    lastInputIndex = index;
}

void MainComponent::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message)
{
    const juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    keyboardState.processNextMidiEvent(message);
    postMessageToList(message, source->getName());
    
}

void MainComponent::handleNoteOn(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float velocity)
{
    if (!isAddingFromMidiInput) {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void MainComponent::handleNoteOff(juce::MidiKeyboardState *, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (!isAddingFromMidiInput) {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void MainComponent::postMessageToList(const juce::MidiMessage &message, const juce::String &source)
{
    (new IncomingMessageCallback(this, message, source))->post();
}

void MainComponent::logMessage(const juce::String &m)
{
    midiMessageBox.moveCaretToEnd();
    midiMessageBox.insertTextAtCaret(m + juce::newLine);
}

void MainComponent::addMessageToList(const juce::MidiMessage &message, const juce::String &source)
{
    auto time = message.getTimeStamp() - startTime;
    
    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)(time)) % 60;
    auto mills = ((int)(time * 1000.0)) % 1000;
    
    auto timecode = juce::String::formatted("%02d:%20d:%02d.%03d", hours, minutes, seconds, mills);
    auto description = getMidiMessageDescription(message);
    
    juce::String midiMessageString(timecode + " - " + description + " (" + source + ")");
    logMessage(midiMessageString);
}

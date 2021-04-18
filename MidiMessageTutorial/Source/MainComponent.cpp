#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 300);
    
    addAndMakeVisible(bassDrumButton);
    bassDrumButton.setButtonText("Bass Drum (36)");
    bassDrumButton.onClick = [this] { setNoteNumber(36); };
    
    addAndMakeVisible(snareDrumButton);
    snareDrumButton.setButtonText("Snare Drum (38");
    snareDrumButton.onClick = [this] { setNoteNumber(38); };
    
    addAndMakeVisible(closedHiHatButton);
    closedHiHatButton.setButtonText("Closed HH (42)");
    closedHiHatButton.onClick = [this] { setNoteNumber(42); };
    
    addAndMakeVisible(openHiHatButton);
    openHiHatButton.setButtonText("Open HH (46)");
    openHiHatButton.onClick = [this] { setNoteNumber(46); };
    
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume (CC7)", juce::dontSendNotification);
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0, 127, 1);
    volumeSlider.onValueChange = [this]
    {
        auto message = juce::MidiMessage::controllerEvent(10, 7, (int)volumeSlider.getValue());
        message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToBuffer(message);
    };
    
    addAndMakeVisible(midiMessagesBox);
    midiMessagesBox.setMultiLine(true);
    midiMessagesBox.setReturnKeyStartsNewLine(true);
    midiMessagesBox.setReadOnly(true);
    midiMessagesBox.setScrollbarsShown(true);
    midiMessagesBox.setCaretVisible(false);
    midiMessagesBox.setPopupMenuEnabled(true);
    midiMessagesBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x32ffffff));
    midiMessagesBox.setColour(juce::TextEditor::outlineColourId, juce::Colour(0x1c000000));
    midiMessagesBox.setColour(juce::TextEditor::shadowColourId, juce::Colour(0x16000000));
    

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
    
    startTimer(1);
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
    auto halfWidth = getWidth() / 2;
    auto buttonsBounds = getLocalBounds().withWidth(halfWidth).reduced(10);
    
    bassDrumButton.setBounds(buttonsBounds.getX(), 10, buttonsBounds.getWidth(), 20);
    snareDrumButton.setBounds(buttonsBounds.getX(), 40, buttonsBounds.getWidth(), 20);
    closedHiHatButton.setBounds(buttonsBounds.getX(), 70, buttonsBounds.getWidth(), 20);
    openHiHatButton.setBounds(buttonsBounds.getX(), 100, buttonsBounds.getWidth(), 20);
    volumeLabel.setBounds(buttonsBounds.getX(), 190, buttonsBounds.getWidth(), 20);
    volumeSlider.setBounds(buttonsBounds.getX(), 220, buttonsBounds.getWidth(), 20);
    
    midiMessagesBox.setBounds(getLocalBounds().withWidth(halfWidth).withX(halfWidth).reduced(10));
}

void MainComponent::setNoteNumber(int noteNumber)
{
    auto message = juce::MidiMessage::noteOn(1, noteNumber, (juce::uint8)100);
    message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToBuffer(message);
    
    auto messageOff = juce::MidiMessage::noteOff(message.getChannel(), message.getNoteNumber());
    messageOff.setTimeStamp(message.getTimeStamp() + 0.1);
    addMessageToBuffer(messageOff);
}

void MainComponent::addMessageToList(const juce::MidiMessage &message)
{
    auto time = message.getTimeStamp();
    
    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)time) % 60;
    auto millis = ((int)(time * 1000.0)) % 1000;
    
    auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
                                            hours,
                                            minutes,
                                            seconds,
                                            millis);
    
    logMessage(timecode + " - " + getMidiMessageDescription(message));
}

void MainComponent::logMessage(const juce::String &m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret(m + juce::newLine);
}

void MainComponent::addMessageToBuffer(const juce::MidiMessage &message)
{
    auto timestamp = message.getTimeStamp();
    auto sampleNumber = (int)(timestamp * sampleRate);
    midiBuffer.addEvent(message, sampleNumber);
}

void MainComponent::timerCallback()
{
    auto currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
    auto currentSampleNumber = (int)(currentTime * sampleRate);
    
    for (const auto metadata : midiBuffer) {
        if (metadata.samplePosition > currentSampleNumber)
            break;
        
        auto message = metadata.getMessage();
        message.setTimeStamp(metadata.samplePosition / sampleRate);
        addMessageToList(message);
    }
    
    midiBuffer.clear(previousSampleNumber, currentSampleNumber - previousSampleNumber);
    previousSampleNumber = currentSampleNumber;
}

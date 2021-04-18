#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    :   synthAudioSource(keyboardState),
        keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 190);
    
    addAndMakeVisible(midiInputListLabel);
    midiInputListLabel.setText("MIDI Input", juce::dontSendNotification);
    midiInputListLabel.attachToComponent(&midiInputList, true);
    
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    
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
        setAudioChannels (0, 2);
    }
    
    startTimer(400);
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
    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    synthAudioSource.getNextAudioBlock(bufferToFill);

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    synthAudioSource.releaseResources();
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
    midiInputList.setBounds(200, 10, getWidth() - 210, 20);
    keyboardComponent.setBounds(10, 40, getWidth() - 20, getHeight() - 50);
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

void MainComponent::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();
    
    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, synthAudioSource.getMidiCollector());
    
    auto newInput = list[index];
    
    if (! deviceManager.isMidiInputDeviceEnabled(newInput.identifier)) {
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);
    }
    deviceManager.addMidiInputDeviceCallback(newInput.identifier, synthAudioSource.getMidiCollector());
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);
    
    lastInputIndex = index;
}


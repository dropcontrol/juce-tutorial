#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
                     true, // showMidiInputOptions must be true
                     true, true, false)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (650, 800);
    
    audioDeviceManager.initialise(0, 2, nullptr, true, {}, nullptr);
    audioDeviceManager.addMidiInputDeviceCallback({}, this);
    audioDeviceManager.addAudioCallback(this);
    
    addAndMakeVisible(audioSetupComp);
    addAndMakeVisible(visualiserViewport);
    
    visualiserViewport.setScrollBarsShown(false, true);
    visualiserViewport.setViewedComponent(&visualiserComp, false);
    visualiserViewport.setViewPositionProportionately(0.5, 0.0);
    
    visualiserInstrument.addListener(&visualiserComp);
    
    for (auto i = 0; i < 15; ++i) {
        synth.addVoice(new MPEDemoSynthVoice());
    }
    
    synth.enableLegacyMode(24);
    synth.setVoiceStealingEnabled(false);
    
    visualiserInstrument.enableLegacyMode(24);
    

    // Some platforms require permissions to open input channels so request that here
//    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
//        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
//    {
//        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
//                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
//    }
//    else
//    {
//        // Specify the number of input and output channels that we want to open
//        setAudioChannels (2, 2);
//    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    audioDeviceManager.removeMidiInputDeviceCallback({}, this);
//    shutdownAudio();
}

//==============================================================================
//void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
//{
//    // This function will be called when the audio device is started, or when
//    // its settings (i.e. sample rate, block size, etc) are changed.
//
//    // You can use this function to initialise any resources you might need,
//    // but be careful - it will be called on the audio thread, not the GUI thread.
//
//    // For more details, see the help for AudioProcessor::prepareToPlay()
//}

//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    // Your audio-processing code goes here!
//
//    // For more details, see the help for AudioProcessor::getNextAudioBlock()
//
//    // Right now we are not producing any data, in which case we need to clear the buffer
//    // (to prevent the output of random noise)
//    bufferToFill.clearActiveBufferRegion();
//}

//void MainComponent::releaseResources()
//{
//    // This will be called when the audio device stops, or when it is being
//    // restarted due to a setting change.
//
//    // For more details, see the help for AudioProcessor::releaseResources()
//}

//==============================================================================
//void MainComponent::paint (juce::Graphics& g)
//{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    // You can add your drawing code here!
//}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    auto visualiserCompWidth = 2000;
    auto visualiserCompHeight = 300;
    
    auto r = getLocalBounds();
    
    visualiserViewport.setBounds(r.removeFromBottom(visualiserCompHeight));
    visualiserComp.setBounds ({visualiserCompWidth, visualiserViewport.getHeight() - visualiserViewport.getScrollBarThickness()});
    
    audioSetupComp.setBounds(r);
}

void MainComponent::handleIncomingMidiMessage(juce::MidiInput *, const juce::MidiMessage &message)
{
    visualiserInstrument.processNextMidiEvent(message);
    midiCollector.addMessageToQueue(message);
}

void MainComponent::audioDeviceAboutToStart(juce::AudioIODevice *device)
{
    auto sampleRate = device->getCurrentSampleRate();
    midiCollector.reset(sampleRate);
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::audioDeviceIOCallback(const float **, int, float **outputChannelData, int numOutputChannels, int numSamples)
{
    // make buffer
    juce::AudioBuffer<float> buffer(outputChannelData, numOutputChannels, numSamples);
    
    // clear it to silence
    buffer.clear();
    juce::MidiBuffer incomingMidi;
    
    // get the MIDI massages for this audio block
    midiCollector.removeNextBlockOfMessages(incomingMidi, numSamples);
    
    // synthesise the block
    synth.renderNextBlock(buffer, incomingMidi, 0, numSamples);
}

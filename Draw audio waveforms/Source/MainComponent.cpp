#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    :   state(Stopped),
        thumbnailCache(5),
        thumbnail(512, formatManager, thumbnailCache)
{
    // Make sure you set the size of the component after
    // you add any child components.
    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };
    
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setEnabled(false);
    
    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setEnabled(false);
    
    setSize (600, 400);
    
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    
    thumbnail.addChangeListener(this);

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
    
    startTimer(40);
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
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    if(readerSource.get() == nullptr)
        bufferToFill.clearActiveBufferRegion();
    else
        transportSource.getNextAudioBlock(bufferToFill);

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
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
    juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
    
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded(g, thumbnailBounds);
    else
        paintIfFileLoaded(g, thumbnailBounds);
}

void MainComponent::paintIfNoFileLoaded(juce::Graphics &g, const juce::Rectangle<int> &thumbnailBounds)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(thumbnailBounds);
    g.setColour(juce::Colours::white);
    g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
}

void MainComponent::paintIfFileLoaded(juce::Graphics &g, const juce::Rectangle<int> &thumnailBounds)
{
    g.setColour(juce::Colours::white);
    g.fillRect(thumnailBounds);
    
    g.setColour(juce::Colours::red);
    
    auto audioLength = (float)thumbnail.getTotalLength();
    thumbnail.drawChannels(g, thumnailBounds, 0.0, audioLength, 1.0f);
    
    g.setColour(juce::Colours::green);
    
    auto audioPosition = (float)transportSource.getCurrentPosition();
    auto drawPosition = (audioPosition / audioLength) * (float)thumnailBounds.getWidth() + (float)thumnailBounds.getX();
    
    g.drawLine(drawPosition, (float)thumnailBounds.getY(), drawPosition, (float)thumnailBounds.getBottom(), 2.0f);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
}

// UI action
void MainComponent::openButtonClicked()
{
    juce::FileChooser chooser("Select a Wave file to play...",
                              {},
                              "*.wav; *.mp3");
    
    if(chooser.browseForFileToOpen())
    {
        juce::File file(chooser.getResult());
        auto* reader = formatManager.createReaderFor(file);
        
        if(reader != nullptr)
        {
            std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled(true);
            thumbnail.setSource(new juce::FileInputSource(file));
            // reset()????????????????????????????????????????????????????????????newSource??????readerSource????????????????????????????????????
            readerSource.reset(newSource.release());
        }
    }
}

void MainComponent::playButtonClicked()
{
    changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

// State and transport
void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;
        
        switch (state) {
            case Stopped:
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);
                break;
                
            case Starting:
                playButton.setEnabled(false);
                transportSource.start();
                break;
            
            case Playing:
                stopButton.setEnabled(true);
                break;
                
            case Stopping:
                transportSource.stop();
                break;
                
            default:
                jassertfalse;
                break;
        }
    }
}

void MainComponent::transportSourceChanged()
{
    changeState(transportSource.isPlaying() ? Playing : Stopped);
}

void MainComponent::thumbnailChanged()
{
    repaint();
}

// callback
void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource) transportSourceChanged();
    if (source == &thumbnail) thumbnailChanged();
}

void MainComponent::timerCallback()
{
    repaint();
}

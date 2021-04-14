#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : Thread("Background Thread")
{
    // Make sure you set the size of the component after
    // you add any child components.
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };
    
    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear");
    clearButton.onClick = [this]{ clearButtonClicked(); };
    
    setSize (300, 200);

    formatManager.registerBasicFormats();
    setAudioChannels (0, 2);
    
    startThread();

//    // Some platforms require permissions to open input channels so request that here
//    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
//        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
//    {
//        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
//                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
//    }
//    else
//    {
//        // Specify the number of input and output channels that we want to open
//        setAudioChannels (0, 2);
//    }
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    stopThread(4000);
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
    
    ReferenceCountedBuffer::Ptr retainedCurrentBuffer(currentBuffer);
    
    if(retainedCurrentBuffer == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    auto* currentAudioSampleBuffer = retainedCurrentBuffer->getAudioSampleBuffer();
    auto position = retainedCurrentBuffer->position;
    
    auto numInputChannels = currentAudioSampleBuffer->getNumChannels();
    auto numOutputChannels = bufferToFill.buffer->getNumChannels();
    
    auto outputSamplesRemaining = bufferToFill.numSamples;
    auto outputSamplesOffset = 0;
    
    while (outputSamplesRemaining > 0)
    {
        auto bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
        auto samplesThisTime = juce::jmin(outputSamplesRemaining, bufferSamplesRemaining);
        
        for (auto channel = 0; channel < numOutputChannels; ++channel)
        {
            bufferToFill.buffer->copyFrom(channel,
                                          bufferToFill.startSample + outputSamplesOffset,
                                          *currentAudioSampleBuffer,
                                          channel % numInputChannels,
                                          position,
                                          samplesThisTime);
        }
        
        outputSamplesRemaining -= samplesThisTime;
        outputSamplesOffset += samplesThisTime;
        position += samplesThisTime;
        
        if (position == currentAudioSampleBuffer->getNumSamples())
            position = 0;
    }
    
    retainedCurrentBuffer->position = position;
    
    
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
    currentBuffer = nullptr;
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
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    clearButton.setBounds(10, 40, getWidth() -20, 20);
}


void MainComponent::run()
{
    while (! threadShouldExit())
    {
        checkForPathToOpen();
        checkForBufferToFree();
        wait(500);
    }
}

void MainComponent::checkForPathToOpen()
{
    juce::String pathToOpen;
    pathToOpen.swapWith(chosenPath);
    
    if (pathToOpen.isNotEmpty())
    {
        juce::File file(pathToOpen);
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
        
        if (reader.get() != nullptr)
        {
            auto duration = (float)reader->lengthInSamples / reader->sampleRate;
            
            if (duration < 2)
            {
                ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                                   (int)reader->numChannels,
                                                                                   (int)reader->lengthInSamples);
                
                reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
                currentBuffer = newBuffer;
                buffers.add(newBuffer);
            }
            else
            {
                // handle the error that the file is 2 seconds or longer..
            }
        }
    }
}

void MainComponent::checkForBufferToFree()
{
    for (auto i = buffers.size(); --i >= 0;)
    {
        ReferenceCountedBuffer::Ptr buffer(buffers.getUnchecked(i));
        
        if(buffer->getReferenceCount() == 2)
            buffers.remove(i);
    }
}

void MainComponent::openButtonClicked()
{
    juce::FileChooser chooser("select a Wave file shorter than 2 seconds to play...",
                              {},
                              "*.wav");
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto path = file.getFullPathName();
        chosenPath.swapWith(path);
        notify();
    }
}

void MainComponent::clearButtonClicked()
{
    currentBuffer = nullptr;
    
}

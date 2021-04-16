#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 100);
    
    levelSliderRight.setRange(0.0, 0.25);
    levelSliderRight.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    levelLabelRight.setText("Noise Right Level", juce::dontSendNotification);
    
    addAndMakeVisible(levelSliderRight);
    addAndMakeVisible(levelLabelRight);
    
    levelSliderLeft.setRange(0.0, 0.25);
    levelSliderLeft.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    levelLabelLeft.setText("Noise Left Level", juce::dontSendNotification);
    
    addAndMakeVisible(levelSliderLeft);
    addAndMakeVisible(levelLabelLeft);
    
    
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
    auto rightLevel = (float)levelSliderRight.getValue();
    auto leftLevel = (float)levelSliderLeft.getValue();
    
//    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
//    {
//        auto *buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
//
//        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
//        {
//            auto noise = random.nextFloat() * 2.0f - 1.0f;
//            buffer[sample] = noise * level;
//        }
//    }

    auto rightLevelScale = rightLevel * 2.0f;
    auto leftLevelScale = leftLevel * 2.0f;
    
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        auto *buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if (channel == 0)
                buffer[sample] = random.nextFloat() * leftLevelScale - leftLevel;
            if (channel == 1)
                buffer[sample] = random.nextFloat() * rightLevelScale - rightLevel;
        }
    }
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
//    bufferToFill.clearActiveBufferRegion();
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
    levelLabelRight.setBounds(10, 10, 120, 20);
    levelSliderRight.setBounds(130, 10, getWidth() - 140, 20);
    levelLabelLeft.setBounds(10, 40, 120, 20);
    levelSliderLeft.setBounds(130, 40, getWidth() - 140, 20);
}

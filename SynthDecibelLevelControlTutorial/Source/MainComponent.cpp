#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 100);
    
    decibelSlider.setRange(-100, -12);
    decibelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    
    decibelSlider.onValueChange = [this] { level = juce::Decibels::decibelsToGain((float)decibelSlider.getValue()); };
    decibelSlider.addListener(this);
    
    decibelSlider.setValue(juce::Decibels::gainToDecibels(level));
    decibelLabel.setText("Noise Level in dB", juce::dontSendNotification);
    
    linearSlider.setRange(0.0, 1.0);
    linearSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
    
    linearSlider.onValueChange = [this] { level = (float)linearSlider.getValue(); };
    linearSlider.addListener(this);
    
    linearSlider.setValue(level);
    linearLebel.setText("Noise level in Linear", juce::dontSendNotification);

    addAndMakeVisible(decibelSlider);
    addAndMakeVisible(decibelLabel);
    addAndMakeVisible(linearSlider);
    addAndMakeVisible(linearLebel);
    

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
    auto currentLevel = level;
    auto levelScale = currentLevel * 2.0f;
    
    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {
        auto *buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
            buffer[sample] = random.nextFloat() * levelScale - currentLevel;
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
    decibelLabel.setBounds(10, 10, 120, 20);
    decibelSlider.setBounds(130, 10, getWidth() - 140, 20);
    linearLebel.setBounds(10, 30, 120, 20);
    linearSlider.setBounds(130, 30, getWidth() - 140, 20);

}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &decibelSlider)
    {
        double gain = juce::Decibels::decibelsToGain(decibelSlider.getValue());
        linearSlider.setValue(gain);
        
    }
    else if (slider == &linearSlider)
    {
        double decibels = juce::Decibels::gainToDecibels(linearSlider.getValue());
        decibelSlider.setValue(decibels);
    }
}


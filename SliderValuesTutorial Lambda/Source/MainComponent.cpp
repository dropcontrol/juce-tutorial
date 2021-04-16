#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 90);
    
    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(50, 5000.0);
    frequencySlider.setTextValueSuffix(" Hz");
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 160, frequencySlider.getTextBoxHeight());
    frequencySlider.setSkewFactorFromMidPoint(500);
    frequencySlider.onValueChange = [this] { durationSlider.setValue(1.0 / frequencySlider.getValue(), juce::dontSendNotification); };
//    frequencySlider.addListener(this);
    
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true);
    
    addAndMakeVisible(durationSlider);
    durationSlider.setRange(1.0 / frequencySlider.getMaximum(), 1.0 / frequencySlider.getMinimum());
    durationSlider.setTextValueSuffix(" s");
    durationSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 160, durationSlider.getTextBoxHeight());
    durationSlider.setSkewFactorFromMidPoint(0.002);
    durationSlider.onValueChange = [this] { durationSlider.setValue(1.0 / frequencySlider.getValue(), juce::dontSendNotification); };
//    durationSlider.addListener(this);
    
    addAndMakeVisible(durationLabel);
    durationLabel.setText("Duration", juce::dontSendNotification);
    durationLabel.attachToComponent(&durationSlider, true);
    
    frequencySlider.setValue(500.0);
    

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
    auto sliderLeft = 120;
    frequencySlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    durationSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
}

//void MainComponent::sliderValueChanged(juce::Slider *slider)
//{
//    if (slider == &frequencySlider)
//        durationSlider.setValue(1.0 / frequencySlider.getValue(), juce::dontSendNotification);
//    else if (slider == &durationSlider)
//        frequencySlider.setValue(1.0 / durationSlider.getValue(), juce::dontSendNotification);
//}

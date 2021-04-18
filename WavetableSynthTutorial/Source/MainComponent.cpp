#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 200);

    cpuUsageLabel.setText("CPU Usage", juce::dontSendNotification);
    cpuUsageText.setJustificationType(juce::Justification::right);
    addAndMakeVisible(cpuUsageLabel);
    addAndMakeVisible(cpuUsageText);
    
    createWavetable();
    
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
    
    startTimer(50);
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
    
    auto numberOfOscillators = 10;
    
    for (auto i = 0; i < numberOfOscillators; ++i) {
//        auto *oscillator = new SineOscillator();
        auto *oscillator = new WavetableOscillator(sineTable);
        auto midiNote = juce::Random::getSystemRandom().nextDouble() * 36.0 + 48.0;
        auto frequency = 440.0 * pow(2.0, (midiNote - 69.0) / 12.0);
        
        oscillator->setFrequency((float)frequency, (float)sampleRate);
        oscillators.add(oscillator);
    }
    level = 0.25f / (float)numberOfOscillators;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!
    
    auto *leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto *rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    bufferToFill.clearActiveBufferRegion();
    
    for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex) {
        auto* oscillator = oscillators.getUnchecked(oscillatorIndex);
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {
            auto levelSample = oscillator->getNextSample() * level;
            
            leftBuffer[sample] += levelSample;
            rightBuffer[sample] += levelSample;
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
    cpuUsageLabel.setBounds(10, 10, getWidth() - 20, 20);
    cpuUsageText.setBounds(10, 10, getWidth() - 20, 20);
}

void MainComponent::timerCallback()
{
    auto cpu = deviceManager.getCpuUsage() * 100;
    cpuUsageText.setText(juce::String(cpu, 6) + " %", juce::dontSendNotification);
}

void MainComponent::createWavetable()
{
    sineTable.setSize(1, (int)tableSize + 1);
    sineTable.clear();
    
    auto *samples = sineTable.getWritePointer(0);
    
    int harmonics[] = { 1, 3, 5, 6, 7, 9, 13, 15 };
    float harmonicWeights[] = { 0.5f, 0.1f, 0.05f, 0.125f, 0.09f, 0.005f, 0.002f, 0.001f };

    jassert(juce::numElementsInArray(harmonics) == juce::numElementsInArray(harmonicWeights));

    for (auto harmonic = 0; harmonic < juce::numElementsInArray(harmonics); ++harmonic) {
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1) * harmonics[harmonic];
        auto currentAngle = 0.0;
        
        for (unsigned int i = 0; i < tableSize; ++i) {
            auto sample = std::sin(currentAngle);
            samples[i] += (float)sample * harmonicWeights[harmonic];
            currentAngle += angleDelta;
        }
    }

    
    samples[tableSize] = samples[0];
}

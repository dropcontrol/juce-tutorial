#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, private juce::Thread
{
public:
    class ReferenceCountedBuffer : public juce::ReferenceCountedObject
    {
    public:
        typedef juce::ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
        
        ReferenceCountedBuffer(const juce::String& nameToUse,
                               int numChannels,
                               int numSamples)
            : name(nameToUse), buffer(numChannels, numSamples)
        {
            DBG(juce::String("Buffer named '") + name + "' constructed. numChannles = " + juce::String(numSamples));
        }
        
        ~ReferenceCountedBuffer()
        {
            DBG(juce::String("Buffer named '") + name + "' destroyed");
        }
        
        juce::AudioSampleBuffer* getAudioSampleBuffer()
        {
            return &buffer;
        }
        
        int position = 0;
        
    private:
        juce::String name;
        juce::AudioSampleBuffer buffer;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReferenceCountedBuffer)
    };

    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void run() override;
    void checkForBufferToFree();
    void checkForPathToOpen();
    
    void openButtonClicked();
    void clearButtonClicked();
    
    //==============================================================================
    // Your private member variables go here...
    juce::TextButton openButton;
    juce::TextButton clearButton;
    
    juce::AudioFormatManager formatManager;
    
    juce::ReferenceCountedArray<ReferenceCountedBuffer> buffers;
    ReferenceCountedBuffer::Ptr currentBuffer;
    
    juce::String chosenPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class CustomLookAndFeel     : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }
};


class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    static juce::String removeSpaces(const juce::String &text);

private:
    //==============================================================================
    // Your private member variables go here...
    CustomLookAndFeel lf;
    juce::Label label            { {}, "This is some label text." };
    juce::TextButton textButton  { "This is a button" };
    juce::TextEditor textEditor;
    juce::Slider slider1;
    juce::Slider slider2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

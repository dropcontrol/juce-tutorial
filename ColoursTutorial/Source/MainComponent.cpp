#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 210);
    
//    label.setColour(juce::Label::textColourId, juce::Colours::black);
    setLookAndFeel(&lf);
    label.setEditable (true);
    addAndMakeVisible (label);

    textEditor.setText ("This is a text editor.");
    addAndMakeVisible (textEditor);

    textButton.setClickingTogglesState (true);
    addAndMakeVisible (textButton);
    
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);
    addAndMakeVisible(slider1);
    addAndMakeVisible(slider2);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setFont (juce::Font (16.0f));
//    g.setColour (juce::Colours::white);
//    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
    
    g.fillAll(juce::Colours::lightgreen);
    g.setColour(juce::Colours::orange);
    
    auto centralArea = getLocalBounds().toFloat().reduced(10.0f);
    g.drawRoundedRectangle(centralArea, 5.0f, 3.0f);
    
    juce::Array<juce::Colour> colours(juce::Colours::red, juce::Colours::green, juce::Colours::blue);
    
//    auto defaultColour = juce::Colours::black;
//    juce::Array<juce::Colour> colours {
//        juce::Colours::findColourForName("red", defaultColour),
//        juce::Colours::findColourForName("green", defaultColour),
//        juce::Colours::findColourForName("blue", defaultColour)
//    };
    
    
//    juce::Array<juce::Colour> colours { juce::Colours::findColourForName (removeSpaces("Dark Red"), defaultColour),
//                                        juce::Colours::findColourForName (removeSpaces("Dark Green"), defaultColour),
//                                        juce::Colours::findColourForName (removeSpaces("Dark Blue"), defaultColour) };

    
    
    auto colorBarArea = centralArea.reduced(4.0f).withHeight(20.0f);
    auto colorArea = colorBarArea.withWidth(colorBarArea.getWidth() / (float)colours.size());
    
    for (auto colour : colours) {
        g.setColour(colour);
        g.fillRect(colorArea);
        
        colorArea.translate(colorArea.getWidth(), 0.0f);
    }
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    label     .setBounds (20, 40,  getWidth() - 40, 20);
    textEditor.setBounds (20, 70,  getWidth() - 40, 20);
    textButton.setBounds (20, 100, getWidth() - 40, 20);
    slider1   .setBounds (20, 130, getWidth() - 40, 20);
    slider2   .setBounds (20, 160, getWidth() - 40, 20);
}

juce::String MainComponent::removeSpaces(const juce::String &text)
{
    return text.removeCharacters(" ");
}

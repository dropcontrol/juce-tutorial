/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class MainComponentTutorialApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    MainComponentTutorialApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // Add your application's initialisation code here..
        mainWindow.reset (new MainWindow (getApplicationName()));

    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        mainWindow = nullptr;

    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)  : DocumentWindow (name,
                                                          juce::Colours::lightgrey,
                                                          DocumentWindow::allButtons)
        {
            setContentOwned(new MainComponent, true);
            setUsingNativeTitleBar (true);
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
            setResizable(true, true);
        }
 
        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
 
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };
    
private:
    std::unique_ptr<MainWindow> mainWindow;

};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainComponentTutorialApplication)

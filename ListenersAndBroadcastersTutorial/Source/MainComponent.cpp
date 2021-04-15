#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (600, 100);
    
    addAndMakeVisible(checkTheTimeButton);
    checkTheTimeButton.setButtonText("Check the time...");
    checkTheTimeButton.addListener(this); //dontSendNotification引数[7]は、ラベルがリスナーを持っている場合に、この変更をブロードキャストするのを防ぎます。(ラベル・オブジェクトは、テキストの編集にも使えるので、リスナーを持つことができます。) この場合、リスナーを持つことができないことは分かっていますが(自分のプライベート・メンバーであるため)、特定しておくことは良い習慣です。
    
    addAndMakeVisible(timeLabel);
    timeLabel.setColour(juce::Label::backgroundColourId, juce::Colours::black);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    timeLabel.setJustificationType(juce::Justification::centred);

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
    checkTheTimeButton.removeListener(this);
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
    checkTheTimeButton.setBounds(10, 10, getWidth() - 20, 40);
    timeLabel.setBounds(10, 60, getWidth() - 20, 40);
}

void MainComponent::buttonClicked(juce::Button *button)
{
    //ここでは、関数に渡されたポインタとボタンのアドレスを比較して、一致するかどうかを確認しています。今回のように、ボタンが1つしかない場合でも、これを行う必要があります。関数に渡された基本のButtonクラスへのポインタと、TextButtonクラスのようなButtonのサブクラスのインスタンスを比較することは安全です。
    if(button == &checkTheTimeButton) //ここでボタンのポインタを比較して当該のボタンかどうかを判断してる
    {
        auto currentTime = juce::Time::getCurrentTime(); //オペレーティングシステムから現在の時刻を取得するためにTimeクラスを使用しています。
        
        auto includeDate = true;
        auto includeTime = true;
        auto currentTimeString = currentTime.toString(includeDate, includeTime); //Timeオブジェクトを読み取り可能な文字列に変換します。2つのbool値により、出力のカスタマイズが可能です（詳細については、Time::toString()関数のドキュメントを参照してください）。
        
        timeLabel.setText(currentTimeString, juce::dontSendNotification); //ここでは、ラベル内に表示されるテキストを更新します。
    }
}

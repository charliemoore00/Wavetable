#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    //_______________________
    freqSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    freqSlider.setTextValueSuffix("Hz");
    freqSlider.setRange(110, 1500, 1);
    freqSlider.setValue(440);
    freqSlider.addListener(this);
    addAndMakeVisible(freqSlider);

    freqLabel.setText("Frequency", juce::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    
    //______________________
    ampSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    ampSlider.setRange(0.0, 1.0, 0.01);
    ampSlider.addListener(this);
    ampSlider.setValue(0.2);
    addAndMakeVisible(ampSlider);
    
    ampLabel.setText("Amplitude", juce::dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);
    
    //______________________
    frequency = freqSlider.getValue();
    phase = 0;
    wtSize = 1024;
    amplitude = ampSlider.getValue();
    

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
    
    currentSampleRate = sampleRate;
    
    //one cycle of a sin wave
    for(int i = 0; i < wtSize; i++)
    {
        waveTable.insert(i, sin(2.0 * juce::MathConstants<double>::pi * i / wtSize));
    }
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //bufferToFill.clearActiveBufferRegion();
    
    //setting up buffers for stereo channels 
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    //float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    
    for(int sample = 0; sample < bufferToFill.numSamples; sample++)
    {
        leftSpeaker[sample] = waveTable[(int)phase] * amplitude;
        //rightSpeaker[sample] = waveTable[(int)phase] * amplitude;
        updateFrequency();
    }
        
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
    
    const int labelSpace = 100;
    freqSlider.setBounds(labelSpace, 20, getWidth()-100, 20);
    ampSlider.setBounds(labelSpace, 50, getWidth()-100, 50);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &freqSlider)
    {
        frequency = freqSlider.getValue();
    } else if (slider == &ampSlider)
    {
        amplitude = ampSlider.getValue();
    }
}

void MainComponent::updateFrequency()
{
    increment = frequency * wtSize / currentSampleRate;
    phase = fmod(phase + increment, wtSize);
}

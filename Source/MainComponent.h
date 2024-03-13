#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Slider::Listener
{
public:
    
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
    void sliderValueChanged(juce::Slider* slider) override;
    void updateFrequency();

private:
    
    juce::Slider freqSlider, ampSlider;
    
    juce::Label freqLabel, ampLabel;
    
    
    //one cycle of the sin function. hold these values in an array
    
    //use this array to access which value of the sin function we're at within the array
    juce::Array<float> waveTable;
    //how big the array is (how many samples make up one cycle)
    double wtSize;
    double frequency;
    double phase;
    double increment;
    double amplitude;
    double currentSampleRate;
    
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

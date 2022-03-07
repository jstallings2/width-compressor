/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VUAnalysis.h"
using namespace juce;

//==============================================================================
/**
*/
class WidthCompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WidthCompressorAudioProcessor();
    ~WidthCompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // public variables for gui components
    // How about a std::map for each band? Seems like a good way to organize
    // Maybe even an array of std::maps?
    // threshold, ratio, attack, gain, mute, solo
    // set slider defaults to these values.
    struct BandParams {
        String name;
        float threshold = 0.0f;
        float ratio = 2.0f;
        int attack = 50;
        int release = 50;
        bool muteOn = false;
        bool soloOn = false;
    };
    
    // Keep an array of parameter sets that we can index
    // Will be bandId - 1 because of zero-based indexing
    // TODO: Do we need to write a constructor for BandParams then? Make sure this gets initialized
    BandParams bands[4];
    
    // Different value for each meter
    std::atomic<float> meterValuesIn[4];
    std::atomic<float> meterValuesOut[4];
    
private:
    
    VUAnalysis vuAnalysis;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidthCompressorAudioProcessor)
};

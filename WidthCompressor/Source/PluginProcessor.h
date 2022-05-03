/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VUAnalysis.h"
using namespace juce;

struct CompressorBand
{
public:
    // Cached versions of our parameters
    juce::AudioParameterFloat* attack { nullptr };
    juce::AudioParameterFloat* release { nullptr };
    juce::AudioParameterFloat* threshold { nullptr };
    juce::AudioParameterChoice* ratio { nullptr };
    juce::AudioParameterBool* bypassed { nullptr };
    
    void prepare(const juce::dsp::ProcessSpec& spec) {
        compressor.prepare(spec);
    }
    void updateCompressorSettings() {
        compressor.setAttack(attack -> get());
        compressor.setRelease(release -> get());
        compressor.setThreshold(threshold -> get());
        compressor.setRatio(ratio -> getCurrentChoiceName().getFloatValue());
    }
    void process(juce::AudioBuffer<float>& buffer) {
        auto block = dsp::AudioBlock<float>(buffer);
        auto context = dsp::ProcessContextReplacing<float>(block);
        
        context.isBypassed = bypassed->get();
        
        compressor.process(context);
    }
private:
    juce::dsp::Compressor<float> compressor;
};

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
    
    using APVTS = juce::AudioProcessorValueTreeState;
    static APVTS::ParameterLayout createParameterLayout();
    
    APVTS apvts {*this, nullptr, "Parameters", createParameterLayout()};
    
private:
    // Part 6 added Compressor band so commenting this out.
//    dsp::Compressor<float> compressor;
//
//    // Cached versions of our parameters
//    juce::AudioParameterFloat* attack { nullptr };
//    juce::AudioParameterFloat* release { nullptr };
//    juce::AudioParameterFloat* threshold { nullptr };
//    juce::AudioParameterChoice* ratio { nullptr };
//    juce::AudioParameterBool* bypassed { nullptr };
    CompressorBand compressor;
    
    
    VUAnalysis vuAnalysis;
    // dsp::LinkwitzRileyFilter<float> HP, LP;
    
    // UNCOMMENT ONE BUT NOT BOTH OF THESE
    // juce::AudioParameterFloat* lowCrossover { nullptr };
    // int lowCrossover = 500; // Hz
    
    // std::array<juce::AudioBuffer<float>, 2> filterBuffers;
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidthCompressorAudioProcessor)
};

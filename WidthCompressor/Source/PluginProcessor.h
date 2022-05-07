/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "VUAnalysis.h"
using namespace juce;

namespace Params {
enum Names {
    Low_Lowmid_Crossover_Freq,
    Lowmid_Mid_Crossover_Freq,
    Mid_High_Crossover_Freq,
    
    Threshold_Low_Band,
    Threshold_Lowmid_Band,
    Threshold_Mid_Band,
    Threshold_High_Band,
    
    Attack_Low_Band,
    Attack_Lowmid_Band,
    Attack_Mid_Band,
    Attack_High_Band,
    
    Release_Low_Band,
    Release_Lowmid_Band,
    Release_Mid_Band,
    Release_High_Band,
    
    Ratio_Low_Band,
    Ratio_Lowmid_Band,
    Ratio_Mid_Band,
    Ratio_High_Band,
    
    Bypassed_Low_Band,
    Bypassed_Lowmid_Band,
    Bypassed_Mid_Band,
    Bypassed_High_Band,
    
    // Global
    global_Gain_In,
};

inline const std::map<Names, juce::String>& GetParams()
{
    static std::map<Names, juce::String> params = {
        {Low_Lowmid_Crossover_Freq, "Low-Lowmid Crossover Freq"},
        {Lowmid_Mid_Crossover_Freq, "Lowmid-Mid Crossover Freq"},
        {Mid_High_Crossover_Freq, "Mid-High Crossover Freq"},
        {Threshold_Low_Band, "Threshold Low Band"},
        {Threshold_Lowmid_Band, "Threshold Lowmid Band"},
        {Threshold_Mid_Band, "Threshold Mid Band"},
        {Threshold_High_Band, "Threshold High Band"},
        {Attack_Low_Band, "Attack Low Band"},
        {Attack_Lowmid_Band, "Attack Lowmid Band"},
        {Attack_Mid_Band, "Attack Mid Band"},
        {Attack_High_Band, "Attack High Band"},
        {Release_Low_Band, "Release Low Band"},
        {Release_Lowmid_Band, "Release Lowmid Band"},
        {Release_Mid_Band, "Release Mid Band"},
        {Release_High_Band, "Release High Band"},
        {Ratio_Low_Band, "Ratio Low Band"},
        {Ratio_Lowmid_Band, "Ratio Lowmid Band"},
        {Ratio_Mid_Band, "Ratio Mid Band"},
        {Ratio_High_Band, "Ratio High Band"},
        {Bypassed_Low_Band, "Bypassed Low Band"},
        {Bypassed_Lowmid_Band, "Bypassed Lowmid Band"},
        {Bypassed_Mid_Band, "Bypassed Mid Band"},
        {Bypassed_High_Band, "Bypassed High Band"},
        {global_Gain_In, "Gain In"}
        
    };
    
    return params;
}
}

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
    
    const float RATIO_MAX = 100.f;
    
    // temporary, for testing correlation values
    float corrDisplay = 0.f;
    
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
    
    // Global DSP Parameters
    juce::dsp::Gain<float> inputGain;
    juce::AudioParameterFloat* inputGainParam;
    
    // juce::dsp::Gain<float> outputGain;
    // juce::AudioParameterFloat* outputGainParam;
    
    template<typename T, typename U>
    void applyGain(T& buffer, U& gain) {
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        gain.process(context);
    }
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidthCompressorAudioProcessor)
};

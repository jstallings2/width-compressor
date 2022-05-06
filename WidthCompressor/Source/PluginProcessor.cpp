/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WidthCompressorAudioProcessor::WidthCompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    using namespace Params;
    const auto& params = GetParams();
    
    auto floatHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName) {
        param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(params.at(paramName)));
        jassert(param != nullptr);
    };
    
    auto choiceHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName) {
        param = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(params.at(paramName)));
        jassert(param != nullptr);
    };
    
    
    auto boolHelper = [&apvts = this->apvts, &params](auto& param, const auto& paramName) {
        param = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(params.at(paramName)));
        jassert(param != nullptr);
    };
    
    floatHelper(compressor.attack, Names::Attack_Low_Band);
    floatHelper(compressor.release, Names::Release_Low_Band);
    floatHelper(compressor.threshold, Names::Threshold_Low_Band);
    floatHelper(inputGainParam, Names::global_Gain_In);
    
    choiceHelper(compressor.ratio, Names::Ratio_Low_Band);
    
    boolHelper(compressor.bypassed, Names::Bypassed_Low_Band);
        
}

WidthCompressorAudioProcessor::~WidthCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String WidthCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WidthCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WidthCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WidthCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WidthCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WidthCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WidthCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WidthCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WidthCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void WidthCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WidthCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    compressor.prepare(spec);
    
    vuAnalysis.setSampleRate(sampleRate);
    /*
    LP.prepare(spec);
    HP.prepare(spec);
    
    for (auto& buffer : filterBuffers) {
        buffer.setSize(spec.numChannels, spec.maximumBlockSize);
    }
     */
    
    inputGain.prepare(spec);
    
    inputGain.setRampDurationSeconds(0.05); // 50 ms
    
    
    
}

void WidthCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WidthCompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WidthCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    // Commenting out for Part 6
//    compressor.setAttack(attack -> get());
//    compressor.setRelease(release -> get());
//    compressor.setThreshold(threshold -> get());
//    compressor.setRatio(ratio -> getCurrentChoiceName().getFloatValue());
//
//
//    auto block = dsp::AudioBlock<float>(buffer);
//    auto context = dsp::ProcessContextReplacing<float>(block);
//
//    context.isBypassed = bypassed->get();
//
//    compressor.process(context);
    
    inputGain.setGainDecibels(inputGainParam->get());
    
    applyGain(buffer, inputGain);
    
    
    compressor.updateCompressorSettings();
    compressor.process(buffer);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    // MULTIBAND STUFF: UNCOMMENT WHEN COMPRESSOR IS WORKING
    // Pick up from Matkat tutorial 2:12:30
    /*
    for (auto& fb : filterBuffers){
        fb = buffer;
    }
    
    // Set cutoff frequencies of filters
    auto cutoff = lowCrossover;
    LP.setCutoffFrequency(cutoff);
    HP.setCutoffFrequency(cutoff);
    
    auto fb0Block = dsp::AudioBlock<float>(filterBuffers[0]);
    auto fb1Block = dsp::AudioBlock<float>(filterBuffers[1]);
    
    auto fb0Ctx = dsp::ProcessContextReplacing<float>(fb0Block);
    auto fb1Ctx = dsp::ProcessContextReplacing<float>(fb1Block);
     
     */
    
    if (bands[0].muteOn) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            for (int n = 0; n < buffer.getNumSamples(); n++) {
                buffer.getWritePointer(channel)[n] = 0.f;
            }
        }
    }
    
    // TODO: Implement correlation meter successfully
    // Might want to store the previously calculated correlation values from last block so that we can use that as the meterIn value.
        float cor = 0.f;
        for (int n = 0; n < buffer.getNumSamples(); ++n) {
            float left = buffer.getWritePointer(0)[n];
            float right = buffer.getWritePointer(1)[n];
            cor += (left * right);
            float oldMeterValue = vuAnalysis.processSample(cor);
            for (auto it = std::begin(meterValuesIn); it != std::end(meterValuesIn); ++it) {
                *it = oldMeterValue;
            }
            
            
            auto y = cor * (bands[0].ratio); // replace with processing
            
            
            buffer.getWritePointer(1)[n] = y;
            float newMeterValue = vuAnalysis.processSample(y);
            for (auto it = std::begin(meterValuesOut); it != std::end(meterValuesOut); ++it) {
                *it = newMeterValue;
            }
            
            
            cor /= buffer.getNumSamples();
        }
    }
    
    // Block that Dr. Tarr wrote to get corr between L & R:
    /*
    float cor = 0.f;
    for (int i = 0; i < buffer.getNumSamples() ; ++i){
        float left = buffer.getWritePointer(0)[i];
        float right = buffer.getWritePointer(1)[i];
        cor += (left * right);
    }
    cor /= buffer.getNumSamples();
     */

//==============================================================================
bool WidthCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WidthCompressorAudioProcessor::createEditor()
{
    return new WidthCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void WidthCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void WidthCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid() ) {
        apvts.replaceState(tree);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout WidthCompressorAudioProcessor::createParameterLayout() {
    APVTS::ParameterLayout layout;
    
    using namespace juce;
    using namespace Params;
    const auto& params = GetParams();
    
    auto gainRange = juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f);
    
    layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::global_Gain_In), params.at(Names::global_Gain_In), gainRange, 0));
    
    layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::Threshold_Low_Band), params.at(Names::Threshold_Low_Band), NormalisableRange<float>(-60.f, -12.f, 1.f, 1.f), 0));
    
    //layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::Threshold_Low_Band), params.at(Names::Threshold_Low_Band), NormalisableRange<float>(-1.f, 1.f), 0));
    
    auto attackReleaseRange = NormalisableRange<float>(5, 1000, 1, 1);
    
    layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::Attack_Low_Band), params.at(Names::Attack_Low_Band), attackReleaseRange, 50));
    
    layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::Release_Low_Band), params.at(Names::Release_Low_Band), attackReleaseRange, 250));
    
    auto choices = std::vector<double>{1, 1.5, 2, 3, 4, 5, 6, 7, 8, 10, 15, 20, 50, 100};
    juce::StringArray sa;
    for (auto choice : choices) {
        sa.add( juce::String(choice, 1));
    }
    
    layout.add(std::make_unique<AudioParameterChoice>(params.at(Names::Ratio_Low_Band), params.at(Names::Ratio_Low_Band), sa, 3));
    
    layout.add(std::make_unique<AudioParameterBool>(params.at(Names::Bypassed_Low_Band), params.at(Names::Bypassed_Low_Band), false));
    
    
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WidthCompressorAudioProcessor();
}

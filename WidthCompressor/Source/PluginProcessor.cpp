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
    // Might want to store the previously calculated correlation values from last block so that we can use that as the meterIn value. - EDIT: VUAnalysis stores that itself
    
    // Obviously the big "IF" here is if we calculated the correlation values correctly. But in a dire situation, if we get it "close" so that everything is between -1 and 1, we should be ok.
    
    compressor.updateCompressorSettings();
    // Start of main DSP algorithm
    
    // TODO: For a lot of reasons, we are going to want to use the compressor.processSample rather than passing a block will be a lot easier and more effieicnt.
    auto bufferSize = buffer.getNumSamples();
    float wValues[bufferSize];
    
    int miniBufferSize = 4;
    int k = 0; // counter variable, reset every time we get to miniBufferSize
    int index = 0;
    int stopIndex = index + miniBufferSize;
    float corrs[bufferSize];
    
    // TODO: Refactor this into a separate method -->
    for (int n = 0; n < buffer.getNumSamples(); ++n) {
        // https://forum.cockos.com/showthread.php?t=126040 for the normalization term!
        // Note: If this normalization term doesn't actually work, we just need to normalize everything between -1 and 1
        
        float cor = 0.f;
        if(stopIndex > bufferSize)
            cor = corrDisplayIn; // last calculated cor value - also use if we don't need to calculate corr again (really only need to calculate at the start of each mini buffer - use the cached version to save time). Use cached if k != index
        else
            cor = xcorr(buffer, index, stopIndex);
            
        k++;

        if (k == stopIndex) {
            index += miniBufferSize;
            stopIndex += miniBufferSize;
            k = 0;
        }
    
        corrs[n] = cor;
        
        // For the meter, leave between -1 and 1
        // TODO: Meter still janky.
        corrDisplayIn = cor;
        float oldMeterValue = vuAnalysis.processSample(cor);
        for (auto it = std::begin(meterValuesIn); it != std::end(meterValuesIn); ++it) {
            *it = oldMeterValue;
        }
        
        // We feed this value to the compressor now.
        /* Note that I've monkeypatched the juce::BallisticsFilter (used in the juce::Compressor) so that it will scale our signal between 0 and 1 without using abs. This is a crucial part of the algorithm because we need to conserve ordinality - we do not have the concept of "same amplitude but opposing phase" as we do with a regular signal. A value that was at -0.3 needs to be considered a lesser value than one that was at 0.3 for example.
         */
        // ^ Note I don't think juce lets you do this. But since abs will do nothing after it's between 0 and 1, we can just do it here.
        cor = (-cor + 1) * 0.5;
        
        float corNew = compressor.processSample(cor);
        
        // Getting this value out of the compressor, we need to scale back to between -1 and 1 for the out meter.
//        float corForDisplay = corNew * 2 - 1;
//
//        float newMeterValue = vuAnalysis.processSample(corForDisplay);
//        //DBG("Meter value: " + (String)(newMeterValue) + "\n");
//
//        for (auto it = std::begin(meterValuesOut); it != std::end(meterValuesOut); ++it) {
//            *it = newMeterValue;
//        }
        
        
        // Give us a scaling factor (w) between new and old, that's what we're actually after
        float w = cor != 0 ? corNew / cor : 1.f; // watches out for div by zero
        wValues[n] = w;
        
    // EOL
    }
        
    
    // <--
    
    
    
    // Should end up with a whole buffer of those values. Then can do mid-side processing
    midSide.processStereoWidth(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples(), wValues);
    
    // calculate correlation again
    float corOut = 0.f;
    if(stopIndex > bufferSize)
        corOut = corrDisplayOut; // last calculated cor value - also use if we don't need to calculate corr again (really only need to calculate at the start of each mini buffer - use the cached version to save time). Use cached if k != index
    else
        corOut = xcorr(buffer, index, stopIndex);
    
    float newMeterValue = vuAnalysis.processSample(corOut);
    //DBG("Meter value: " + (String)(newMeterValue) + "\n");
    
    
    
    for (auto it = std::begin(meterValuesOut); it != std::end(meterValuesOut); ++it) {
        *it = newMeterValue;
    }
    
    DBG(meterValuesIn[0] - meterValuesOut[0]);

    
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
    
    layout.add(std::make_unique<AudioParameterFloat>(params.at(Names::Threshold_Low_Band), params.at(Names::Threshold_Low_Band), NormalisableRange<float>(0.f, 2.f, 0.1f, 1.f), 0));
    
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

float WidthCompressorAudioProcessor::xcorr(AudioBuffer<float>& buffer, int index, int stopIndex) {
    
    auto left = buffer.getReadPointer(0);
    auto right = buffer.getReadPointer(1);

    // calc regular xcorr (might have to do more complex math to do this)
    float c = 0.f;
        
    float cxx0 = 0.f;
    float cyy0 = 0.f;
    // sum squares left & right
    for (int k = index; k < stopIndex; k++) {
        int reverseIndex = stopIndex - 1 - (k - index);
        c = left[k] * right[reverseIndex];
        cxx0 += pow(fabs(left[k]), 2);
        cyy0 += pow(fabs(right[k]), 2);
    }
    // sqrt those
    float scale = 1.f;
    if (cxx0 != 0.f && cyy0 != 0.f)
        scale = sqrt(cxx0*cyy0);
    else if (cxx0 != 0.f)
        scale = sqrt(cxx0);
    else if (cyy0 != 0.f)
        scale = sqrt(cyy0);
    // else leave it as 1
    
    c /= scale;
    
    return c;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WidthCompressorAudioProcessor();
}

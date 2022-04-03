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
    vuAnalysis.setSampleRate(sampleRate);
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

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if (bands[0].muteOn) {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            for (int n = 0; n < buffer.getNumSamples(); n++) {
                buffer.getWritePointer(channel)[n] = 0.f;
            }
        }
    }
    
    // For testing purposes, this is treating Band 1's ratio knob as a gain knob and the two meters for each band as regular in/out gain meters. Just to make sure things are hooked up.
        float cor = 0.f;
        for (int n = 0; n < buffer.getNumSamples(); ++n) {
            float oldMeterValue = vuAnalysis.processSample(x, channel);
            for (auto it = std::begin(meterValuesIn); it != std::end(meterValuesIn); ++it) {
                *it = oldMeterValue;
            }
            float left = buffer.getWritePointer(0)[i];
            float right = buffer.getWritePointer(1)[i];
            cor += (left * right);
            
            auto y = x * (bands[0].ratio); // replace with processing
            
            
            buffer.getWritePointer(channel)[n] = y;
            float newMeterValue = vuAnalysis.processSample(y, channel);
            for (auto it = std::begin(meterValuesOut); it != std::end(meterValuesOut); ++it) {
                *it = newMeterValue;
            }
            
            
            
            
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
}

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
}

void WidthCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WidthCompressorAudioProcessor();
}

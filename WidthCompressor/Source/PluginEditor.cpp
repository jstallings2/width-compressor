/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WidthCompressorAudioProcessorEditor::WidthCompressorAudioProcessorEditor (WidthCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    setSize(1000, 500);
    
    addAndMakeVisible(band1ControlPanel);
    addAndMakeVisible(band2ControlPanel);
    
    // Meter things
    
    simpleMeter.setBounds(25, 25, 10, 100);
    simpleMeter.configuration = SimpleMeter::VERTICAL;
    addAndMakeVisible(simpleMeter);
    
    startTimerHz(30);
    
    
    
    
}

WidthCompressorAudioProcessorEditor::~WidthCompressorAudioProcessorEditor()
{
}

//==============================================================================
void WidthCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.drawFittedText((String)(getHeight() / numBands), 400, 400, 50, 50, juce::Justification::horizontallyCentred, 1);

}

void WidthCompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    band1ControlPanel.setBounds(0, 0, 600, getBandHeight());
    band2ControlPanel.setBounds(0, band1ControlPanel.getBottom(), 600, getBandHeight());
}

int WidthCompressorAudioProcessorEditor::getBandHeight() {
    return getHeight() / numBands;
}

void WidthCompressorAudioProcessorEditor::timerCallback() {
    simpleMeter.update(audioProcessor.meterValue);
}

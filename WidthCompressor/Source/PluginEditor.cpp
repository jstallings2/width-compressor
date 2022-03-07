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
    
    // Hook the different bands up to the processor so they can each listen for events / update the processor when needed
    // TODO: If every component ends up requiring this, should just subclass Component and then make all components that type "AwareComponent" or something
    band1ControlPanel.setBandId(1);
    band2ControlPanel.setBandId(2);
    band3ControlPanel.setBandId(3);
    band4ControlPanel.setBandId(4);
    
    band1ControlPanel.linkToProcessor(p);
    band2ControlPanel.linkToProcessor(p);
    band3ControlPanel.linkToProcessor(p);
    band4ControlPanel.linkToProcessor(p);
    
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    addAndMakeVisible(band1ControlPanel);
    addAndMakeVisible(band2ControlPanel);
    addAndMakeVisible(band3ControlPanel);
    addAndMakeVisible(band4ControlPanel);
    
    vizFeedbackPanel.setNumBands(numBands);
    vizFeedbackPanel.setBounds(600, 0, getWidth() - 600, getHeight());
    addAndMakeVisible(vizFeedbackPanel);
    
    
    // Start the timer which all our visualizers listen to.
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
    band3ControlPanel.setBounds(0, band2ControlPanel.getBottom(), 600, getBandHeight());
    band4ControlPanel.setBounds(0, band3ControlPanel.getBottom(), 600, getBandHeight());
    vizFeedbackPanel.setBounds(600, 0, getWidth() - 600, getHeight());
    
}

// TODO: Make band height a field & add a setter
int WidthCompressorAudioProcessorEditor::getBandHeight() {
    return getHeight() / numBands;
}

void WidthCompressorAudioProcessorEditor::timerCallback() {
    // need to call update for every meter, a lot of copy-paste right now but when we have each band keep its own meters we'll just tell the bands to update their two meters each
    vizFeedbackPanel.updateAllMeters(audioProcessor.meterValuesIn, "in");
    vizFeedbackPanel.updateAllMeters(audioProcessor.meterValuesOut, "out");
    //simpleMeter.update(audioProcessor.meterValue);
}



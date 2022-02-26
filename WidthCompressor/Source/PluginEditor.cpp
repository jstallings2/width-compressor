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
    
    
    setSize (1000, 500);
    
    addAndMakeVisible(compControls);
    
    
    // mute button
    muteButton.setButtonText("Mute");
    addAndMakeVisible(muteButton);
    
    soloButton.setButtonText("Solo");
    addAndMakeVisible(soloButton);
}

WidthCompressorAudioProcessorEditor::~WidthCompressorAudioProcessorEditor()
{
}

//==============================================================================
void WidthCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void WidthCompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    compControls.setBounds(0, 0, 600, 200);
    
    muteButton.setBounds(compControls.getRight() + 20, 150, 100, 40);
    soloButton.setBounds(compControls.getRight() + 20, muteButton.getBottom() - 5, 100, 40);

    
}

void WidthCompressorAudioProcessorEditor::buttonClicked(Button *button) {
    return;
}

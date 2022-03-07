/*
  ==============================================================================

    BandControlPanel.cpp
    Created: 25 Feb 2022 9:15:06pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BandControlPanel.h"

//==============================================================================
BandControlPanel::BandControlPanel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    compControls.setMyParent(this);
    addAndMakeVisible(compControls);
    
    // mute button
    muteButton.setButtonText("Mute");
    addAndMakeVisible(muteButton);
    
    soloButton.setButtonText("Solo");
    addAndMakeVisible(soloButton);

}

BandControlPanel::~BandControlPanel()
{
}

void BandControlPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("BandControlPanel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void BandControlPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    compControls.setBounds(0, 0, 525, 200);
    
    muteButton.setBounds(compControls.getRight() + 10, 65, 100, 40);
    soloButton.setBounds(compControls.getRight() + 10, muteButton.getBottom() - 5, 100, 40);

}

void BandControlPanel::buttonClicked(Button *button) {
    return;
}

// Set up a pointer to the processor so we can read/write dsp parameters to/from it
// Our parent component should call this when initializing us.
void BandControlPanel::linkToProcessor(WidthCompressorAudioProcessor &p) {
    audioProcessor = &p;
}

void BandControlPanel::setBandId(const int newId) {
    bandId = newId;
}

void BandControlPanel::sliderValueChanged(Slider* slider) {
    // Need to pick our band out of the array of bands in processor
    WidthCompressorAudioProcessor::BandParams* params = &(audioProcessor->bands[bandId - 1]);
    
    // Figure out which knob it was
    // TODO: CompControls is doing nothing but make this inconvenient, consider refactoring so a BandControlPanel is the direct parent of its knobs and buttons and get rid of compControls
    String knob = compControls.identifyThisSlider(slider);
    
    if (knob == "threshold")
        params->threshold = slider->getValue();
    if(knob == "ratio")
        params->ratio = slider->getValue();
    if (knob == "attack")
        params->attack = slider->getValue();
    if (knob == "release")
        params->release = slider->getValue();
}


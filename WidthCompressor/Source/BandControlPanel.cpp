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
BandControlPanel::BandControlPanel(juce::AudioProcessorValueTreeState& apvts)
{
    using namespace Params;
    const auto& params = GetParams();
    
    auto makeAttachmentHelper = [&params, &apvts](auto& attachment, const auto& name, auto& slider){
        makeAttachment(attachment, apvts, params, name, slider);
    };
    
    // Add more controls here
    makeAttachmentHelper(thresholdKnobAttachment, Names::Threshold_Low_Band, thresholdKnob);
    makeAttachmentHelper(ratioKnobAttachment, Names::Ratio_Low_Band, ratioKnob);
    makeAttachmentHelper(attackKnobAttachment, Names::Attack_Low_Band, attackKnob);
    makeAttachmentHelper(releaseKnobAttachment, Names::Release_Low_Band, releaseKnob);
    
    
    // threshold
    thresholdKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    thresholdKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    thresholdKnob.setTitle("Threshold");
    thresholdKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(thresholdKnob);
    
    // ratio
    ratioKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ratioKnob.setSkewFactorFromMidPoint(5.f);
    ratioKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    ratioKnob.setTitle("Ratio");
    ratioKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(ratioKnob);
    
    // attack
    attackKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    attackKnob.setTitle("Attack");
    attackKnob.setSkewFactorFromMidPoint(250.f);
    attackKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(attackKnob);
    
    // release
    releaseKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    releaseKnob.setTitle("Release");
    releaseKnob.setSkewFactorFromMidPoint(250.f);
    releaseKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(releaseKnob);
    
    // mute button
    //muteButton.addListener(this);
    muteButton.setButtonText("Mute");
    addAndMakeVisible(muteButton);
    
    // soloButton
    //soloButton.addListener(this);
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
    
    g.setColour(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.drawRect(getX()+1, 1, 1, getHeight() - 2);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("BandControlPanel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
    
    g.drawFittedText(thresholdKnob.getTitle(), thresholdKnob.getX(), thresholdKnob.getY() - 20, thresholdKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(ratioKnob.getTitle(), ratioKnob.getX(), ratioKnob.getY() - 20, ratioKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(attackKnob.getTitle(), attackKnob.getX(), attackKnob.getY()  -20, attackKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(releaseKnob.getTitle(), releaseKnob.getX(), releaseKnob.getY() -20, releaseKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    
    g.setFont(11.0f);
    // Draw labels
    int labelYOffset = -20; // always negative
    g.drawFittedText((String)thresholdKnob.getMinimum(), thresholdKnob.getX(), thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)thresholdKnob.getMaximum(), thresholdKnob.getRight() - 10, thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)ratioKnob.getMinimum(), ratioKnob.getX(), ratioKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)audioProcessor->RATIO_MAX, ratioKnob.getRight() - 10, thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)attackKnob.getMinimum(), attackKnob.getX(), attackKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)attackKnob.getMaximum(), attackKnob.getRight() - 10, attackKnob.getBottom() + labelYOffset, 20, 10, juce::Justification::horizontallyCentred, 1);
    
    g.drawFittedText((String)releaseKnob.getMinimum(), releaseKnob.getX(), releaseKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)releaseKnob.getMaximum(), releaseKnob.getRight() - 10, releaseKnob.getBottom() + labelYOffset, 20, 10, juce::Justification::horizontallyCentred, 1);
    
    g.setFont(24.f);
    g.drawFittedText((String) audioProcessor->corrDisplayIn, thresholdKnob.getX(), thresholdKnob.getY(), 200, 100, juce::Justification::horizontallyCentred, 1);
}

void BandControlPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    muteButton.setBounds(525 + 10, 65, 100, 40);
    soloButton.setBounds(525 + 10, muteButton.getBottom() - 5, 100, 40);
    
    
    // Comp controls
    thresholdKnob.setBounds(25, 25, 125, 125);
    ratioKnob.setBounds(175, 25, 125, 125);
    attackKnob.setBounds(325, (thresholdKnob.getHeight() / 2.f), 75, 75);
    releaseKnob.setBounds(425, (thresholdKnob.getHeight() / 2.f), 75, 75);
}

// Set up a pointer to the processor so we can read/write dsp parameters to/from it
// Our parent component should call this when initializing us.
void BandControlPanel::linkToProcessor(WidthCompressorAudioProcessor &p) {
    audioProcessor = &p;
}

void BandControlPanel::setBandId(const int newId) {
    bandId = newId;
}

// Listener stuff - may not be needed with apvts in play
/*
 
void BandControlPanel::sliderValueChanged(Slider* slider) {
    // Need to pick our band out of the array of bands in processor
    WidthCompressorAudioProcessor::BandParams* params = &(audioProcessor->bands[bandId - 1]);
    
    // Figure out which knob it was
    // TODO: CompControls is doing nothing but make this inconvenient, consider refactoring so a BandControlPanel is the direct parent of its knobs and buttons and get rid of compControls
    if(slider == &thresholdKnob)
        params->threshold = slider->getValue();
    if (slider == &ratioKnob)
        params->ratio = slider->getValue();
    if (slider == &attackKnob)
        params->attack = slider->getValue();
    if (slider == &releaseKnob)
        params->release = slider->getValue();
}

void BandControlPanel::buttonClicked(Button *button) {
    // Need to pick our band out of the array of bands in processor
    WidthCompressorAudioProcessor::BandParams* params = &(audioProcessor->bands[bandId - 1]);
    if (button == &muteButton)
        params->muteOn = !params->muteOn;
    if (button == &soloButton)
        params->soloOn = !params->soloOn;
}
 
 */


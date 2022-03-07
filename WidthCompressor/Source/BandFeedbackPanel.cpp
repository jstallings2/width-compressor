/*
  ==============================================================================

    BandFeedbackPanel.cpp
    Created: 3 Mar 2022 10:55:47pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BandFeedbackPanel.h"

//==============================================================================
BandFeedbackPanel::BandFeedbackPanel()
{
    inMeter.configuration = SimpleMeter::HORIZONTAL;
    outMeter.configuration = SimpleMeter::HORIZONTAL;
    addAndMakeVisible(inMeter);
    addAndMakeVisible(outMeter);

}

BandFeedbackPanel::~BandFeedbackPanel()
{
}

void BandFeedbackPanel::paint (juce::Graphics& g)
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
}

void BandFeedbackPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    inMeter.setBounds(getX() + 25, 25, 350, 20);
    outMeter.setBounds(getX() + 25, 75, 350, 20);

}

void BandFeedbackPanel::updateMeter(SimpleMeter &meter, std::atomic<float> &newValue) {
    meter.update(newValue);
}

void BandFeedbackPanel::setBandHeight(int newBandHeight) {
    bandHeight = newBandHeight;
}

SimpleMeter& BandFeedbackPanel::getInMeter() {
    return inMeter;
}

SimpleMeter& BandFeedbackPanel::getOutMeter() {
    return outMeter;
}

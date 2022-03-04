/*
  ==============================================================================

    CompControls.cpp
    Created: 25 Feb 2022 7:32:42pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CompControls.h"

//==============================================================================
CompControls::CompControls()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    // threshold
    thresholdKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    thresholdKnob.setRange(0.f, 1.f);
    thresholdKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    thresholdKnob.setTitle("Threshold");
    addAndMakeVisible(thresholdKnob);
    
    // ratio
    ratioKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ratioKnob.setRange(1.f, 30.f);
    ratioKnob.setSkewFactorFromMidPoint(5.5f);
    ratioKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    ratioKnob.setTitle("Ratio");
    addAndMakeVisible(ratioKnob);
    
    // attack
    attackKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackKnob.setRange(5, 1000); // ms
    attackKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    attackKnob.setTitle("Attack");
    addAndMakeVisible(attackKnob);
    
    // release
    releaseKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseKnob.setRange(5, 1000); // ms
    releaseKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    releaseKnob.setTitle("Release");
    addAndMakeVisible(releaseKnob);

}

CompControls::~CompControls()
{
}

void CompControls::paint (juce::Graphics& g)
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
    // g.drawText ("CompControls", getLocalBounds(),
                // juce::Justification::centred, true);   // draw some placeholder text
    
    g.drawFittedText(thresholdKnob.getTitle(), thresholdKnob.getX(), thresholdKnob.getY() - 20, thresholdKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(ratioKnob.getTitle(), ratioKnob.getX(), ratioKnob.getY() - 20, ratioKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(attackKnob.getTitle(), attackKnob.getX(), attackKnob.getY()  -20, attackKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText(releaseKnob.getTitle(), releaseKnob.getX(), releaseKnob.getY() -20, releaseKnob.getWidth(), 30, juce::Justification::horizontallyCentred, 1);
}

void CompControls::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    thresholdKnob.setBounds(25, 25, 125, 125);
    ratioKnob.setBounds(175, 25, 125, 125);
    attackKnob.setBounds(325, (thresholdKnob.getHeight() / 2.f), 75, 75);
    releaseKnob.setBounds(425, (thresholdKnob.getHeight() / 2.f), 75, 75);

}

void CompControls::sliderValueChanged(Slider *slider) {
    return;
}

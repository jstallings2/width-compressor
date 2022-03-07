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
    thresholdKnob.setRange(-1.f, 1.f);
    thresholdKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    thresholdKnob.setTitle("Threshold");
    thresholdKnob.setValue(0.f);
    thresholdKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(thresholdKnob);
    
    // ratio
    ratioKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    ratioKnob.setRange(1.f, 30.f);
    ratioKnob.setSkewFactorFromMidPoint(5.5f);
    ratioKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    ratioKnob.setTitle("Ratio");
    ratioKnob.setValue(2.f);
    ratioKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(ratioKnob);
    
    // attack
    attackKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    attackKnob.setRange(5, 1000); // ms
    attackKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    attackKnob.setTitle("Attack");
    attackKnob.setValue(50);
    attackKnob.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(attackKnob);
    
    // release
    releaseKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    releaseKnob.setRange(5, 1000); // ms
    releaseKnob.setTextBoxStyle(Slider::NoTextBox, false, 100, 30);
    releaseKnob.setTitle("Release");
    releaseKnob.setValue(50);
    releaseKnob.setPopupDisplayEnabled(true, false, this);
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
    
    g.setFont(11.0f);
    // Draw labels
    int labelYOffset = -20; // always negative
    g.drawFittedText((String)thresholdKnob.getMinimum(), thresholdKnob.getX(), thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)thresholdKnob.getMaximum(), thresholdKnob.getRight() - 10, thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)ratioKnob.getMinimum(), ratioKnob.getX(), ratioKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)ratioKnob.getMaximum(), ratioKnob.getRight() - 10, thresholdKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)attackKnob.getMinimum(), attackKnob.getX(), attackKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)attackKnob.getMaximum(), attackKnob.getRight() - 10, attackKnob.getBottom() + labelYOffset, 20, 10, juce::Justification::horizontallyCentred, 1);
    
    g.drawFittedText((String)releaseKnob.getMinimum(), releaseKnob.getX(), releaseKnob.getBottom() + labelYOffset, 10, 10, juce::Justification::horizontallyCentred, 1);
    g.drawFittedText((String)releaseKnob.getMaximum(), releaseKnob.getRight() - 10, releaseKnob.getBottom() + labelYOffset, 20, 10, juce::Justification::horizontallyCentred, 1);
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

String CompControls::identifyThisSlider(Slider *slider) {
    if(slider == &thresholdKnob)
        return "threshold";
    if (slider == &ratioKnob)
        return "ratio";
    if (slider == &attackKnob)
        return "attack";
    if (slider == &releaseKnob)
        return "release";
}

void CompControls::setMyParent(Slider::Listener* parent) {
    myParent = parent;
    thresholdKnob.addListener(myParent);
    ratioKnob.addListener(myParent);
    attackKnob.addListener(myParent);
    releaseKnob.addListener(myParent);
}

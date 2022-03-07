/*
  ==============================================================================

    VizFeedbackPanel.cpp
    Created: 2 Mar 2022 5:03:32pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VizFeedbackPanel.h"

//==============================================================================
VizFeedbackPanel::VizFeedbackPanel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // setNumBands(4);
    bandHeight = WINDOW_HEIGHT / numBands;
    // vizFeedbackPanelWidth =  WINDOW_WIDTH - 600;
    band1FeedbackPanel.setBandHeight(bandHeight);
    band1FeedbackPanel.setBounds(getX(), 0, vizFeedbackPanelWidth, bandHeight);
    band2FeedbackPanel.setBounds(getX(), band1FeedbackPanel.getBottom(), vizFeedbackPanelWidth, bandHeight);
    band3FeedbackPanel.setBounds(getX(), band2FeedbackPanel.getBottom(), vizFeedbackPanelWidth, bandHeight);
    band4FeedbackPanel.setBounds(getX(), band3FeedbackPanel.getBottom(), vizFeedbackPanelWidth, bandHeight);
    addAndMakeVisible(band1FeedbackPanel);
    addAndMakeVisible(band2FeedbackPanel);
    addAndMakeVisible(band3FeedbackPanel);
    addAndMakeVisible(band4FeedbackPanel);
    

}

VizFeedbackPanel::~VizFeedbackPanel()
{
}

void VizFeedbackPanel::paint (juce::Graphics& g)
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
    //g.drawText ("VizFeedbackPanel", getLocalBounds(),
                //juce::Justification::centred, true);   // draw some placeholder text
    //String text = (String)getX() + " " + (String)getWidth() + " " +  (String)getBandHeight() + " " + (String)band1FeedbackPanel.getX() + " " +  (String)band1FeedbackPanel.getY() + " " +  (String)band1FeedbackPanel.getWidth() + " " +  (String)band1FeedbackPanel.getHeight();
    //g.drawText(text, getLocalBounds(),
                //juce::Justification::centred, true);   // draw some placeholder text
 
}

void VizFeedbackPanel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    

}

/*
    updateAllMeters()
    handles update events from the Editor to all meters on the panel.
    The flow of the event is Processor -> Editor -> VizFeedbackPanel -> SimpleMeter
    Called with the timer frequency (i.e. 30 times a second)
    This should take in a pointer to an array or map of meterValues from the processor. (Right now it is just one value).
 */
void VizFeedbackPanel::updateAllMeters(std::atomic<float>* newMeterValues, String inOrOut) {
    
    // when doing multiple meters, each meter will have a different value stored in the processor. Therefore we will call update for each meter and set it using its corresponding value from the processor. We get these values into this function as a (pointer to an) array of std::atomic<float>'s from the processor that has been passed to us by the editor.
    // This method gets called twice, once for inputs and then once for outputs
    // TODO: Ask Tarr if having things being passed down through components like this is bad / slow
    if (inOrOut == "in") {
        band1FeedbackPanel.updateMeter(band1FeedbackPanel.getInMeter(), newMeterValues[0]);
        band2FeedbackPanel.updateMeter(band2FeedbackPanel.getInMeter(), newMeterValues[1]);
        band3FeedbackPanel.updateMeter(band3FeedbackPanel.getInMeter(), newMeterValues[2]);
        band4FeedbackPanel.updateMeter(band4FeedbackPanel.getInMeter(), newMeterValues[3]);
    }
    else {
        // out
        band1FeedbackPanel.updateMeter(band1FeedbackPanel.getOutMeter(), newMeterValues[0]);
        band2FeedbackPanel.updateMeter(band2FeedbackPanel.getOutMeter(), newMeterValues[1]);
        band3FeedbackPanel.updateMeter(band3FeedbackPanel.getOutMeter(), newMeterValues[2]);
        band4FeedbackPanel.updateMeter(band4FeedbackPanel.getOutMeter(), newMeterValues[3]);
    }
}

int VizFeedbackPanel::getBandHeight() {
    return bandHeight / numBands;
}

void VizFeedbackPanel::setNumBands(int newNumBands) {
    numBands = newNumBands;
}

void VizFeedbackPanel::setLookAndFeelReferences(LookAndFeel_V4* b1LaF, LookAndFeel_V4* b2LaF, LookAndFeel_V4* b3LaF, LookAndFeel_V4* b4LaF) {
    // set internal fields
    band1LookAndFeel = b1LaF;
    band2LookAndFeel = b2LaF;
    band3LookAndFeel = b3LaF;
    band4LookAndFeel = b4LaF;
    // hook up the bands to corresponding look and feel references
    band1FeedbackPanel.setLookAndFeel(band1LookAndFeel);
    band2FeedbackPanel.setLookAndFeel(band2LookAndFeel);
    band3FeedbackPanel.setLookAndFeel(band3LookAndFeel);
    band4FeedbackPanel.setLookAndFeel(band4LookAndFeel);
}

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
void VizFeedbackPanel::updateAllMeters(std::atomic<float> &newMeterValue) {
    
    // when doing multiple meters, each meter will have a different value stored in the processor. Therefore we will call update for each meter and set it using its corresponding value from the processor. We get these values into this function as a (pointer to an) array of std::atomic<float>'s from the processor that has been passed to us by the editor.
    // for each band:
        // update IN meter
        // update OUT meter
    // TODO: Ask Tarr if having things being passed down through components like this is bad / slow
    // For now we are just passing this value but in the end will need to pass two
    band1FeedbackPanel.updateMeters(newMeterValue);
}

int VizFeedbackPanel::getBandHeight() {
    return bandHeight / numBands;
}

void VizFeedbackPanel::setNumBands(int newNumBands) {
    numBands = newNumBands;
}

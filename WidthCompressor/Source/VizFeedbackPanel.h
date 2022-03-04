/*
  ==============================================================================

    VizFeedbackPanel.h
    Created: 2 Mar 2022 5:03:32pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BandFeedbackPanel.h"

//==============================================================================
/*
*/
class VizFeedbackPanel  : public juce::Component
{
public:
    VizFeedbackPanel();
    ~VizFeedbackPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getBandHeight();
    
    void setNumBands(int newNumBands);
    
    // Meter setting stuffs
    void updateAllMeters(std::atomic<float> &newMeterValue);
    
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 1000;

private:
    
    // I think putting values here that the band1FeedbackPanel constructor would need, before declaring the band1FeedbackPanel, was crucial to it working.
    int numBands = 2;
    int bandHeight = WINDOW_HEIGHT / numBands;
    int vizFeedbackPanelWidth = WINDOW_WIDTH - 600;
    
    BandFeedbackPanel band1FeedbackPanel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VizFeedbackPanel)
};

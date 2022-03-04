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

private:
    
    
    int numBands = 2;
    int bandHeight = WINDOW_HEIGHT / numBands;
    
    BandFeedbackPanel band1FeedbackPanel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VizFeedbackPanel)
};

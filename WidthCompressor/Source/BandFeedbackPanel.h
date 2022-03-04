/*
  ==============================================================================

    BandFeedbackPanel.h
    Created: 3 Mar 2022 10:55:47pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SimpleMeter.h"

//==============================================================================
/*
*/
class BandFeedbackPanel  : public juce::Component
{
public:
    BandFeedbackPanel();
    ~BandFeedbackPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateMeters(std::atomic<float> &newMeterValue);
    void setBandHeight(int newBandHeight);

private:
    
    SimpleMeter simpleMeter;
    
    int bandHeight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandFeedbackPanel)
};

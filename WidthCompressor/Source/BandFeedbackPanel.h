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
    
    SimpleMeter& getInMeter();
    SimpleMeter& getOutMeter();
    void updateMeter(SimpleMeter &meter, std::atomic<float> &newValue);
    void setBandHeight(int newBandHeight);

private:
    
    SimpleMeter inMeter;
    SimpleMeter outMeter;
    
    int bandHeight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandFeedbackPanel)
};

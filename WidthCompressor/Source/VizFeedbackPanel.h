/*
  ==============================================================================

    VizFeedbackPanel.h
    Created: 2 Mar 2022 5:03:32pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SimpleMeter.h"

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
    
    // Meter setting stuff
    void updateAllMeters(std::atomic<float> &newMeterValue);

private:
    
    SimpleMeter simpleMeter;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VizFeedbackPanel)
};

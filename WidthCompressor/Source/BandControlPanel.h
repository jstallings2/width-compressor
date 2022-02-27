/*
  ==============================================================================

    BandControlPanel.h
    Created: 25 Feb 2022 9:15:06pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CompControls.h"

//==============================================================================
/*
*/
class BandControlPanel  : public juce::Component,
public juce::ToggleButton::Listener
{
public:
    BandControlPanel();
    ~BandControlPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    CompControls compControls;
    ToggleButton muteButton;
    ToggleButton soloButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandControlPanel)
};

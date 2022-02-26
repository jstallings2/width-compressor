/*
  ==============================================================================

    CompControls.h
    Created: 25 Feb 2022 7:32:42pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
class CompControls  : public juce::Component, public juce::Slider::Listener
{
public:
    CompControls();
    ~CompControls() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
    
private:
    Slider thresholdKnob;
    Slider ratioKnob;
    Slider attackKnob;
    Slider releaseKnob;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompControls)
};

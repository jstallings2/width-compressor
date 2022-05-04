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
class CompControls  : public juce::Component
{
public:
    CompControls();
    ~CompControls() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    String identifyThisSlider(Slider* slider);
    
    void setMyParent(Slider::Listener* parent);
    
    
    
    
private:
    Slider thresholdKnob;
    Slider ratioKnob;
    Slider attackKnob;
    Slider releaseKnob;
    
    Slider::Listener* myParent;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompControls)
};

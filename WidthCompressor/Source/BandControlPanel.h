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
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class BandControlPanel  : public juce::Component,
public juce::ToggleButton::Listener,
public juce::Slider::Listener
{
public:
    BandControlPanel();
    ~BandControlPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;
    
    void linkToProcessor(WidthCompressorAudioProcessor &p);
    
    void setBandId(const int newId);
    
    

private:
//    CompControls compControls;
    Slider thresholdKnob;
    Slider ratioKnob;
    Slider attackKnob;
    Slider releaseKnob;
    
    ToggleButton muteButton;
    ToggleButton soloButton;
    
    int bandId;
    
    // A pointer to 
    WidthCompressorAudioProcessor* audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandControlPanel)
};

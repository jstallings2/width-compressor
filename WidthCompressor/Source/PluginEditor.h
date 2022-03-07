/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CompControls.h"
#include "BandControlPanel.h"
#include "VizFeedbackPanel.h"


//==============================================================================
/**
*/

class WidthCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor,
public Timer
{
public:
    WidthCompressorAudioProcessorEditor (WidthCompressorAudioProcessor&);
    ~WidthCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // These methods should update fields dependent on number of bands and add them to the GUI
    void addBand();
    void removeBand();
    int getBandHeight();
    
    const int WINDOW_HEIGHT = 600;
    const int WINDOW_WIDTH = 1000;
    
private:
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WidthCompressorAudioProcessor& audioProcessor;
    BandControlPanel band1ControlPanel; // lowest frequency, always visible
    
    // Increasing frequency, only visible if numBands is set to 2, 3, or 4.
    BandControlPanel band2ControlPanel;
    BandControlPanel band3ControlPanel;
    BandControlPanel band4ControlPanel;
    
    int numBands = 4; // number of bands used, default 4.
    
    // Visual Feedback stuff
    VizFeedbackPanel vizFeedbackPanel;
        
    void timerCallback() override;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidthCompressorAudioProcessorEditor)
};


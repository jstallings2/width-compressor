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
template<
    typename Attachment,
    typename APVTS,
    typename Params,
    typename ParamName,
    typename SliderType
>
void makeAttachment(std::unique_ptr<Attachment>& attachment,
                    APVTS& apvts,
                    const Params& params,
                    const ParamName& name,
                    SliderType& slider
                    ) {
    attachment = std::make_unique<Attachment>(apvts, params.at(name), slider);
}

class BandControlPanel  : public juce::Component
{
public:
    BandControlPanel(juce::AudioProcessorValueTreeState& apvts);
    ~BandControlPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
//    void buttonClicked(Button* button) override;
//    void sliderValueChanged(Slider* slider) override;
    
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
    
    // Slider Attachments
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> thresholdKnobAttachment,
                                ratioKnobAttachment,
                                attackKnobAttachment,
                                releaseKnobAttachment;
    
    int bandId;
    
    // A pointer to 
    WidthCompressorAudioProcessor* audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandControlPanel)
};

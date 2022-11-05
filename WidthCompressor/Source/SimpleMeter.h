//
//  SimpleMeter.h
//
//
//  Copyright © 2019 Eric Tarr. All rights reserved.
//  Code may not be distributed without permission
//
//  Based on Inter-App Audio Plugin (https://docs.juce.com/master/tutorial_plugin_examples.html )
//
//  This class creates a meter component which draws a value between 0 and 1

#ifndef SimpleMeter_h
#define SimpleMeter_h
#include <JuceHeader.h>

using namespace juce;
// Class definition
class SimpleMeter  : public juce::Component,
private juce::Timer
{
public:
    SimpleMeter()
    {
        startTimerHz (30);
    }
    
    //==============================================================================
    void paint (Graphics& g) override
    {
        //g.fillAll (Colours::transparentBlack);
        
        auto area = g.getClipBounds();
        
    
        auto unfilledHeight = area.getHeight() * (1.0 - fabs(level));
        auto unfilledWidth = area.getWidth() * (1.0 - fabs(level));
        auto correlationWidth = area.getWidth() * fabs(level-0.5f);

        switch (configuration) {
            case VERTICAL:
                g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                g.reduceClipRegion (area.getX(), area.getY(),
                                    area.getWidth(), (int) unfilledHeight);
                g.setColour (getLookAndFeel().findColour (Slider::trackColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                break;
            
            case HORIZONTAL:
                g.setColour (getLookAndFeel().findColour (Slider::trackColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                if (level >= 0) {
                    g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                    // fill to the right
                    // do i need to indicate the unfilled area rather than the filled area?
                    g.reduceClipRegion (area.getWidth() / 2 + (1-unfilledWidth), area.getY(),
                                    unfilledWidth,area.getHeight());
                }
                else {
                    g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                    // fill to the left
//                    g.reduceClipRegion (area.getX() + (int) unfilledWidth, area.getY(),
//                                    area.getWidth() / 2 - (int) unfilledWidth,area.getHeight());
                    g.reduceClipRegion(area.getX(), area.getY(), unfilledWidth, area.getHeight());
                }
//                g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
//                g.fillRoundedRectangle (area.toFloat(), 6.0);
                break;
            
            case GAINREDUCTION:
                g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                g.reduceClipRegion (area.getX(), area.getY(),
                                    area.getWidth() - (int) unfilledWidth,area.getHeight());
                g.setColour (getLookAndFeel().findColour (Slider::trackColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                break;
                
            case CORRELATION:
                g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                g.setColour (getLookAndFeel().findColour (Slider::trackColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                if (level > 0.5){
                    g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
                    //g.reduceClipRegion (area.getX(), area.getY(),
                    //                area.getWidth()/2.0,area.getHeight());
                    g.fillRect((int)(area.getX() + area.getWidth()/2.0),
                               (int)area.getY(),
                               (int)correlationWidth,
                               (int)area.getHeight());
                    //g.reduceClipRegion (area.getX()+area.getWidth()/2.0+(int)correlationWidth, area.getY(),
                                    //area.getWidth(),area.getHeight());
                }
                else{
                    g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
                    g.fillRect((int)(area.getX() + area.getWidth()/2.0 - correlationWidth),
                               (int)area.getY(),
                               (int)correlationWidth,
                               (int)area.getHeight());
                }
                    //g.reduceClipRegion (area.getWidth()/2.0 - (int) correlationWidth, area.getY(),correlationWidth,area.getHeight());
                   
                
                //g.fillRoundedRectangle (area.toFloat(), 6.0);
                break;
            default: // Vertical
                g.drawFittedText((String)level, area.getX() + 100, area.getY() - 2, 100, 50, juce::Justification::horizontallyCentred, 1);
                g.setColour (getLookAndFeel().findColour (Slider::thumbColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                g.reduceClipRegion (area.getX(), area.getY(),
                                    area.getWidth(), (int) unfilledHeight);
                g.setColour (getLookAndFeel().findColour (Slider::trackColourId));
                g.fillRoundedRectangle (area.toFloat(), 6.0);
                
                break;
        }
        
        
    }
    
    void resized() override {}
    
    //==============================================================================
    // Called from the audio thread.
    void update (float newLevel)
    {
        if (newLevel >= 0) {
        // make sure it doesn't go above the maximum possible level of 1
            maxLevel = jmin (newLevel,1.0f);
        }
        else {
            maxLevel = jmax (newLevel, -1.0f);
        }
        maxLevel = 0.5f * maxLevel + 0.5f;
    }
    
    enum Configuration {HORIZONTAL,VERTICAL,GAINREDUCTION,CORRELATION};
    Configuration configuration = CORRELATION;
    
private:
    //==============================================================================
    void timerCallback() override
    {
        auto callbackLevel = maxLevel.load();//exchange(1.0);
        
        if (fabs(callbackLevel) > 0.000001)
            level = callbackLevel;
        else
            level = 0; // Round down to zero, if the value is very small
        
        repaint();
    }
    
    std::atomic<float> maxLevel { 0.0f };
    float level = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleMeter)
};


#endif 

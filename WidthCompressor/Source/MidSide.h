/*
  ==============================================================================

    MidSide.h
    Created: 27 Mar 2022 4:11:30pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MidSide {
  
public:
    
    void processStereoWidth(float &L, float &R, float width);
    void processStereoWidth(float* leftChannel, float* rightChannel, const int N, float* widthValues);
    void processStereowidth(juce::AudioBuffer<float>& buffer);
    
    void midSideEncode(const float &L, const float &R, float &M, float &S);
    void midSideDecode(const float &M, const float &S, float &L, float &R);
    
    void setStereoWidthValue(float newWidth);
    
};


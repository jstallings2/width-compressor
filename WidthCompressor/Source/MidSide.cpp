/*
  ==============================================================================

    MidSide.cpp
    Created: 27 Mar 2022 4:11:30pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include "MidSide.h"

void MidSide::midSideEncode(const float &L, const float &R, float &M, float &S) {
    M = L + R;
    S = L - R;
}

void MidSide::midSideDecode(const float &M, const float &S, float &L, float &R) {
    L = 0.5f*(M + S);
    R = 0.5f*(M - S);
}

void MidSide::processStereoWidth(float &L, float &R, float width) {
    float M = 0.f;
    float S = 0.f;
    
    // L,R go in : M,S come out
    midSideEncode(L, R, M, S);
    
    S *= width;
    M *= (2.f - width);
    
    midSideDecode(M, S, L, R);
    
}

void MidSide::processStereoWidth(float *leftChannel, float *rightChannel, const int N, float* widthValues) {
    for (int n = 0; n < N; n++) {
        float L = leftChannel[n];
        float R = rightChannel[n];
        processStereoWidth(L, R, widthValues[n]);
        leftChannel[n] = L;
        rightChannel[n] = R;
    }
}

void MidSide::processStereowidth(juce::AudioBuffer<float> &buffer) {
    float * leftChannel = buffer.getWritePointer(0);
    float * rightChannel = buffer.getWritePointer(1);
    int N = buffer.getNumSamples();
    midSide.processStereoWidth(leftChannel, rightChannel, N);
}


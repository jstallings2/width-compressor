/*
  ==============================================================================

    WidthCompressorDSP.cpp
    Created: 3 Apr 2022 12:04:38pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#include "WidthCompressorDSP.h"
#include "Utils.h"

/**
 This is the main entry point of the DSP of the plugin. This will be called from the AudioProcessor and we will pass pointers to the left and right channel of the current buffer, of size N. Our job here is to split the signal up into filtered bands and call the appropriate method for each of the bands. Then, put it back together and pass it back to the processor.
 */
void WidthCompressorDSP::process(float *leftChannel, float *rightChannel, const int bufferSize) {
    // splitIntoBands(int order, int numBands)
    
    // following lines adapted from multibandStereoComp.m
    int order = 2; // filter order
    int numBands = 4;
    
    // Create logarithmically spaced bands
    auto logspace = pyLogspace(1, 4, numBands+1);
    for (int i = 0; i < logspace.size(); ++i) {
        logspace[i] *= 2;
    }
    
    // Smoothing filter gain
    float g = 0.992f;
    
    // Initialize feedback delay
    float fb = 0.f;
    
    // 3D Array to store the filtered signal for each band: bufferSize x numBands x 2 channels
    float filteredSignal[bufferSize][numBands][2];
    
    // Filter signal into multiple bands
    // Need a substitute for Matlab "butter" - check juce_dsp module
    for (int i = 0; i < numBands; i++) {
        
    }
    
    
    
    
    
    
    
    
    
}

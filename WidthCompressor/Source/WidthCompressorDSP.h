/*
  ==============================================================================

    WidthCompressorDSP.h
    Created: 3 Apr 2022 12:04:38pm
    Author:  Jacob Stallings

  ==============================================================================
*/

#pragma once

/**
 * The main WidthCompressorDSP object's job is to take the input signal and split it into frequency bands, delegate the actual processing of a band to a different object, then get the filtered output back from all bands and put it back together into the output signal.
 */

public class WidthCompressorDSP {
public:
    WidthCompressorDSP();
    
    void prepare(float newFs);
    
    float processSample(float x, int c);
    
private:
    // sample rate
    float Fs = 48000.f;
    
    // array of pointers to the SingleBandWidthCompressor objects
};

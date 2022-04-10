/*
  ==============================================================================

    Utils.h
    Created: 3 Apr 2022 5:16:37pm
    Author:  Jacob Stallings
 
    This file contains helper functions and function-objects for doing various things.

  ==============================================================================
*/

#pragma once


/**
 Generate an array of logarithmically spaced out numbers, meant to recreate the logspace function from Matlab.
 Used to create filter bands for multiband processing.
 Taken verbatim from: https://stackoverflow.com/questions/21429294/is-there-something-like-numpy-logspace-in-c
 */
template<typename T>
class Logspace {
private:
    T curValue, base;

public:
    Logspace(T first, T base) : curValue(first), base(base) {}

    T operator()() {
        T retval = curValue;
        curValue *= base;
        return retval;
    }
};

std::vector<double> pyLogspace(double start, double stop, int num = 50, double base = 10) {
    double realStart = pow(base, start);
    double realBase = pow(base, (stop-start)/num);

    std::vector<double> retval;
    retval.reserve(num);
    std::generate_n(std::back_inserter(retval), num, Logspace<double>(realStart,realBase));
    return retval;
}

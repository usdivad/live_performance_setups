#include "LowPassFilter.h"

#include <cmath>

LowPassFilter::LowPassFilter()
{
}

LowPassFilter::~LowPassFilter()
{
}

float LowPassFilter::processSample(float x, int channel)
{
    
    float y;
    
    // Audio EQ formula using coefficients
    y = b0 * x +
        b1 * x1[channel] +
        b2 * x2[channel] +
        (-a1) * y1[channel] +
         (-a2) * y2[channel];
    
    
    // Apply delay for samples n-2 and n-1
    x2[channel] = x1[channel];
    x1[channel] = x;
    
    // Do same thing for y
    y2[channel] = y1[channel];
    y1[channel] = y;

    return y;
}

void LowPassFilter::setFs(float inFs)
{
    Fs = inFs;
    _updateCoefficients();
}

void LowPassFilter::setFrequency(float inFreq)
{
    if (inFreq != freq) {
        _updateCoefficients();
        freq = inFreq;
    }
}

void LowPassFilter::_updateCoefficients()
{
    // Intermediate variables
    float w0 = (2.f * M_PI) * freq / Fs;
    float alpha = sin(w0) / (2.f * Q);
    
    float cos_w0 = cos(w0); // So that we only have to calculate this once
    
    // Coefficients
    b0 = (1.f - cos_w0) * 0.5f;
    b1 = 1.f - cos_w0;
    b2 = b1;
    a0 = 1.f + alpha;
    a1 = -2.f * cos_w0;
    a2 = 1.f - alpha;
    
    // Divide everything by a0
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    
    a1 /= a0;
    a2 /= a0;
}

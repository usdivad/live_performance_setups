#include "AllPassFilter.h"

AllPassFilter::AllPassFilter()
{
}

AllPassFilter::AllPassFilter(float delayInSamples, float speed)
{
    z.setDelaySamples(delayInSamples);
    z.setSpeed(speed);
}

AllPassFilter::~AllPassFilter()
{
}

float AllPassFilter::processSample(float x, int channel)
{
    
    float y;
    
    // Delay lines in + feedback w -g
    float inZ = x + (feedbackGain * -1.0f * fb[channel]);
    
    // Delay lines out
    float outZ = z.processSample(inZ, channel);
    
    // Output calc w g
    y = outZ + (feedbackGain * inZ);
    
    // Feedback signal result
    fb[channel] = y;

    return y;
}

void AllPassFilter::setFs(float Fs)
{
    this->Fs = Fs;
    z.setFs(Fs);
}

void AllPassFilter::setFeedbackGain(float g)
{
    this->feedbackGain = g;
}

void AllPassFilter::setModulation(float m)
{
    this->modulationAmount = m;
    z.setDepth(m);
}

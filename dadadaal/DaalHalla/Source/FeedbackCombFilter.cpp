#include "FeedbackCombFilter.h"

FeedbackCombFilter::FeedbackCombFilter()
{
}

FeedbackCombFilter::FeedbackCombFilter(float delayInSamples, float speed)
{
    z.setDelaySamples(delayInSamples);
    z.setSpeed(speed);
}

FeedbackCombFilter::~FeedbackCombFilter()
{
}

float FeedbackCombFilter::processSample(float x, int channel)
{
    
    float y;
    
    // Delay lines in + feedback w -g
    // (can try with positive g for diff results)
    float inZ = x + (feedbackGain * -1.0f * fb[channel]);
    
    // Delay lines out
    float outZ = z.processSample(inZ, channel);
    
    // Output calc w g
    y = outZ;
    
    // Feedback signal result
    fb[channel] = y;

    return y;
}

void FeedbackCombFilter::setFs(float Fs)
{
    this->Fs = Fs;
    z.setFs(Fs);
}

void FeedbackCombFilter::setFeedbackGain(float g)
{
    this->feedbackGain = g;
}

void FeedbackCombFilter::setModulation(float m)
{
    this->modulationAmount = m;
    z.setDepth(m);
}
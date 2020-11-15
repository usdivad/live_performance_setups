#include "FeedbackCombFilter.h"

FeedbackCombFilter::FeedbackCombFilter()
{
}

FeedbackCombFilter::FeedbackCombFilter(float delayInSamples, float speed)
{
    z.setDelaySamples(delayInSamples);
    z.setSpeed(speed);
}

FeedbackCombFilter::FeedbackCombFilter(float delayInSamples, float speed, float apfDelay, float apfSpeed)
{
    z.setDelaySamples(delayInSamples);
    z.setSpeed(speed);
    
    apf.setDelay(apfDelay);
    apf.setSpeed(apfSpeed);
    
    // These APF params are hard-coded for now
    apf.setFeedbackGain(0.22f);
    apf.setModulation(3.03f);
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
    // fb[channel] = y; // Direct output->input
    // fb[channel] = lpf.processSample(outZ, channel); // With LPF processing
    // fb[channel] = apf.processSample(outZ, channel); // With APF processing
    fb[channel] = lpf.processSample(apf.processSample(outZ, channel), channel); // APF -> LPF!

    return y;
}

void FeedbackCombFilter::setFs(float Fs)
{
    this->Fs = Fs;
    z.setFs(Fs);
    apf.setFs(Fs);
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

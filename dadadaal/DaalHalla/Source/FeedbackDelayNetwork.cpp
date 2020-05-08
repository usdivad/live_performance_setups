#include "FeedbackDelayNetwork.hpp"

FeedbackDelayNetwork::FeedbackDelayNetwork()
{
    z1.setDelaySamples(1777.0f);
    z1.setDelaySamples(2221.0f);
}

FeedbackDelayNetwork::~FeedbackDelayNetwork()
{
}

float FeedbackDelayNetwork::processSample(float x, int channel)
{
    
    float y;
    
    float inZ1 = x + fb11[channel] + fb21[channel];
    float inZ2 = x + fb12[channel]; + fb22[channel];
    
    float outZ1 = z1.processSample(inZ1, channel);
    float outZ2 = z2.processSample(inZ2, channel);
    
    y = 0.5f * (outZ1 + outZ2);
    
    fb11[channel] = outZ1 * g11 * feedbackGain;
    fb12[channel] = outZ1 * g12 * feedbackGain;
    fb21[channel] = outZ2 * g21 * feedbackGain;
    fb22[channel] = outZ2 * g22 * feedbackGain;
        
    return y;
}

void FeedbackDelayNetwork::setFs(float Fs)
{
    this->Fs = Fs;
    z1.setFs(Fs);
    z2.setFs(Fs);
}

void FeedbackDelayNetwork::setTime(float t)
{
    this->feedbackGain = t;
}

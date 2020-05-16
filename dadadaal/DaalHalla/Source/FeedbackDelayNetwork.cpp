#include "FeedbackDelayNetwork.h"

FeedbackDelayNetwork::FeedbackDelayNetwork()
{
}

FeedbackDelayNetwork::~FeedbackDelayNetwork()
{
}

float FeedbackDelayNetwork::processSample(float x, int channel)
{
    
    float y;
    
    // ========
    // Original 2-line criss-cross
    /*
    float inZ1 = x + fb11[channel] + fb21[channel];
    float inZ2 = x + fb12[channel]; + fb22[channel];
    
    float outZ1 = z1.processSample(inZ1, channel);
    float outZ2 = z2.processSample(inZ2, channel);
     
    fb11[channel] = outZ1 * g11 * feedbackGain;
    fb12[channel] = outZ1 * g12 * feedbackGain;
    fb21[channel] = outZ2 * g21 * feedbackGain;
    fb22[channel] = outZ2 * g22 * feedbackGain;
    
    y = 0.5f * (outZ1 + outZ2);
    */
    
    // =========
    // 4-line using feedback matrix
    
    // Delay lines in + feedback
    float inZ1 = x + fb1[channel];
    float inZ2 = x + fb2[channel];
    float inZ3 = x + fb3[channel];
    float inZ4 = x + fb4[channel];
    
    // Delay lines out
    float outZ1 = z1.processSample(inZ1, channel);
    float outZ2 = z2.processSample(inZ2, channel);
    float outZ3 = z3.processSample(inZ3, channel);
    float outZ4 = z4.processSample(inZ4, channel);
    
    // Fedback signal result
    fb1[channel] = ((outZ1 * g11) + (outZ2 * g12) + (outZ3 * g13) + (outZ4 * g14)) * feedbackGain;
    fb2[channel] = ((outZ1 * g21) + (outZ2 * g22) + (outZ3 * g23) + (outZ4 * g24)) * feedbackGain;
    fb3[channel] = ((outZ1 * g31) + (outZ2 * g32) + (outZ3 * g33) + (outZ4 * g34)) * feedbackGain;
    fb4[channel] = ((outZ1 * g41) + (outZ2 * g42) + (outZ3 * g43) + (outZ4 * g44)) * feedbackGain;
    
    // Output for this sample
    y = 0.25f * (outZ1 + outZ2 + outZ3 + outZ4);

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

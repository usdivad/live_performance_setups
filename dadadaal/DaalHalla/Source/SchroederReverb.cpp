#include "SchroederReverb.h"

SchroederReverb::SchroederReverb()
{
}

SchroederReverb::~SchroederReverb()
{
}

float SchroederReverb::processSample(float x, int channel)
{
    
    float y;
    
    // FBCFs in parallel
    float outFBCF1 = m_FBCF1.processSample(x, channel);
    float outFBCF2 = m_FBCF1.processSample(x, channel);
    float outFBCF3 = m_FBCF1.processSample(x, channel);
    float outFBCF4 = m_FBCF1.processSample(x, channel);
    
    // Sum them up
    float summedFBCFs = outFBCF1 + outFBCF2 + outFBCF3 + outFBCF4;
    summedFBCFs *= 0.25f; // Scale amp
    
    // Pass to APFs in series
    float outAPF1 = m_APF1.processSample(summedFBCFs, channel);
    float outAPF2 = m_APF1.processSample(outAPF1, channel);
    
    // Result
    y = outAPF2;

    return y;
}

void SchroederReverb::setFs(float Fs)
{
    this->Fs = Fs;
    
    m_FBCF1.setFs(Fs);
    m_FBCF2.setFs(Fs);
    m_FBCF3.setFs(Fs);
    m_FBCF4.setFs(Fs);
    
    m_APF1.setFs(Fs);
    m_APF2.setFs(Fs);}

void SchroederReverb::setFeedbackGain(float g)
{
    this->feedbackGain = g;
    
    m_FBCF1.setFeedbackGain(g);
    m_FBCF2.setFeedbackGain(g);
    m_FBCF3.setFeedbackGain(g);
    m_FBCF4.setFeedbackGain(g);
}

void SchroederReverb::setDiffusionGain(float g)
{
    this->diffusionGain = g;
    
    m_APF1.setFeedbackGain(g);
    m_APF2.setFeedbackGain(g);
}

void SchroederReverb::setModulation(float m)
{
    this->modulationAmount = m;
    
    m_FBCF1.setModulation(m); // depth
    m_FBCF2.setModulation(m);
    m_FBCF3.setModulation(m);
    m_FBCF4.setModulation(m);
    
    m_APF1.setModulation(m);
    m_APF2.setModulation(m);
}

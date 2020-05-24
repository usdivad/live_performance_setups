#ifndef SchroederReverb_hpp
#define SchroederReverb_hpp

#include "AllPassFilter.h"
#include "FeedbackCombFilter.h"
using namespace std;

class SchroederReverb
{
    
public:
    
    SchroederReverb();
        
    ~SchroederReverb();
    
    float processSample(float x,int channel);
    
    void setFs(float Fs);
    
    void setFeedbackGain(float g);
    
    void setDiffusionGain(float g);
    
    void setModulation(float m);
    
private:
    
    float Fs = 48000.f;
        
    FeedbackCombFilter m_FBCF1 {1426.0f, 0.7f};
    FeedbackCombFilter m_FBCF2 {1781.0f, 1.2f};
    FeedbackCombFilter m_FBCF3 {1973.0f, 0.9f};
    FeedbackCombFilter m_FBCF4 {2098.0f, 1.1f};
    
    AllPassFilter m_APF1 {240.0f, 0.777f};
    AllPassFilter m_APF2 {82.0f, 0.888f};
    
    float feedbackGain = 1.0f;
    float diffusionGain = 1.0f;
    
    float modulationAmount = 0.0f; // depth
    
};



#endif /* SchroederReverb_hpp */

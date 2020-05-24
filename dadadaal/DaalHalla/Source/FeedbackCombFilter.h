#ifndef FeedbackCombFilter_hpp
#define FeedbackCombFilter_hpp

#include "FractionalDelay.h"
using namespace std;

class FeedbackCombFilter
{
    
public:
    
    FeedbackCombFilter();
    
    FeedbackCombFilter(float delayInSamples, float speed);
    
    ~FeedbackCombFilter();
    
    float processSample(float x,int channel);
    
    void setFs(float Fs);
    
    void setFeedbackGain(float g);
    
    void setModulation(float m);
    
private:
    
    float Fs = 48000.f;
        
    FractionalDelay z {250.0f, 0.7f};
    
    float feedbackGain = 1.0f;
    
    float modulationAmount = 0.0f; // depth

    float fb[2] = {0.0f}; // L and R

};



#endif /* FeedbackCombFilter_hpp */

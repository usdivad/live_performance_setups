#ifndef AllPassFilter_hpp
#define AllPassFilter_hpp

#include "FractionalDelay.h"
using namespace std;

class AllPassFilter
{
    
public:
    
    AllPassFilter();
    
    AllPassFilter(float delayInSamples, float speed);
    
    ~AllPassFilter();
    
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



#endif /* AllPassFilter_hpp */

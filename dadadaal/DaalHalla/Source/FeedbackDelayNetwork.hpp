#ifndef FeedbackDelayNetwork_hpp
#define FeedbackDelayNetwork_hpp

#include "FractionalDelay.hpp"
using namespace std;

class FeedbackDelayNetwork
{
    
public:
    
    FeedbackDelayNetwork();
    
    ~FeedbackDelayNetwork();
    
    float processSample(float x,int channel);
    
    void setFs(float Fs);
    
    void setTime(float t);
    
private:
    
    float Fs = 48000.f;
        
    FractionalDelay z1;
    FractionalDelay z2;
    
    float feedbackGain = 0.5f;
    
    float g11 = -1.0f; // gab where a = input, b = output
    float g12 = 1.0f;
    float g21 = 1.0f;
    float g22 = -1.0f;
    
    float fb11[2] = {0.0f}; // L and R
    float fb12[2] = {0.0f};
    float fb21[2] = {0.0f};
    float fb22[2] = {0.0f};

};



#endif /* FeedbackDelayNetwork_hpp */

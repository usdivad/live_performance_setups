#ifndef FeedbackDelayNetwork_hpp
#define FeedbackDelayNetwork_hpp

#include "FractionalDelay.h"
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
        
    FractionalDelay z1 {1777.0f, 0.4f};
    FractionalDelay z2 {2221.0f, 0.8f};
    FractionalDelay z3 {3331.0f, 1.2f};
    FractionalDelay z4 {4111.0f, 1.6f};
    
    float feedbackGain = 1.0f;
    
    // ========
    // Feedback matrix
    // gab where a = input, b = output
    
    // For 2-line
    // float g11 = -1.0f;
    // float g12 = 1.0f;
    // float g21 = 1.0f;
    // float g22 = -1.0f;
    
    // For 4-line
    // Stautner and Puckette (1982)
    float g11 = 0.0f;
    float g12 = 1.0f;
    float g13 = 1.0f;
    float g14 = 0.0f;
    
    float g21 = -1.0f;
    float g22 = 0.0f;
    float g23 = 0.0f;
    float g24 = -1.0f;
    
    float g31 = 1.0f;
    float g32 = 0.0f;
    float g33 = 0.0f;
    float g34 = -1.0f;
    
    float g41 = 0.0f;
    float g42 = 1.0f;
    float g43 = -1.0f;
    float g44 = 0.0f;
    
    // ========
    // Feedback line
    
    // For 2-line
    // float fb11[2] = {0.0f}; // L and R
    // float fb12[2] = {0.0f};
    // float fb21[2] = {0.0f};
    // float fb22[2] = {0.0f};
    
    // For 4-line
    float fb1[2] = {0.0f}; // L and R
    float fb2[2] = {0.0f};
    float fb3[2] = {0.0f};
    float fb4[2] = {0.0f};

};



#endif /* FeedbackDelayNetwork_hpp */

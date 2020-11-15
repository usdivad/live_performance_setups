#ifndef FeedbackCombFilter_hpp
#define FeedbackCombFilter_hpp

#include "FractionalDelay.h"
#include "AllPassFilter.h"

using namespace std;

class FeedbackCombFilter
{
    
public:
    
    FeedbackCombFilter();
    
    FeedbackCombFilter(float delayInSamples, float speed);
    
    FeedbackCombFilter(float delayInSamples, float speed, float apfDelay, float apfSpeed);
    
    ~FeedbackCombFilter();
    
    float processSample(float x,int channel);
    
    void setFs(float Fs);
    
    void setFeedbackGain(float g);
    
    void setModulation(float m);
    
private:
    
    // Simple LPF
    // All the calculations are relative to Nyquist, so we don't have to
    // worry about sample rate
    class LowPassFilter
    {
    public:
        float processSample(float x, int channel)
        {
            float y = ((1.0f-d) * x) + (d * ff[channel]); // Process for output
            ff[channel] = x; // Update feed-forward with new input
            return y;
        }
    private:
        float ff[2] = {0.0f}; // Feed-forward
        float d = 0.5f; // TODO: Parameterize this
    };
    
    // ========
    
    float Fs = 48000.f;
        
    FractionalDelay z {250.0f, 0.7f};
    
    float feedbackGain = 1.0f;
    
    float modulationAmount = 0.0f; // depth

    float fb[2] = {0.0f}; // L and R
    
    LowPassFilter lpf;
    
    AllPassFilter apf;

};



#endif /* FeedbackCombFilter_hpp */

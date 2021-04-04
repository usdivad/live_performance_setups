#ifndef LowPassFilter_hpp
#define LowPassFilter_hpp

/** See https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    for more implementation details */
class LowPassFilter
{
    
public:
    
    LowPassFilter();
    
    ~LowPassFilter();
    
    float processSample(float x, int channel);
    
    void setFs(float inFs);
    
    void setFrequency(float inFreq);
    
private:
    
    void _updateCoefficients();
    
    // ====
    
    float Fs = 48000.f;
    
    float freq = 20000.f;
    float Q = 0.7071f; // sqrt(2)/2. Higher for more resonance
    
    // Coefficients
    float b0 = 1.f; // Pass the signal through w/o any processing
    float b1 = 0.f;
    float b2 = 0.f;
    
    float a0 = 0.f; // Pass the signal through w/o any processing
    float a1 = 0.f;
    float a2 = 0.f;

    // Delay blocks
    float x1[2] = {0.f}; // L and R
    float x2[2] = {0.f};
    float y1[2] = {0.f};
    float y2[2] = {0.f};

};



#endif /* LowPassFilter_hpp */

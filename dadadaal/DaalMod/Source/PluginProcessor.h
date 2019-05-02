/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MAX_DELAY_TIME_IN_SECONDS 2
#define DELAY_TIME_SMOOTH_AMOUNT 0.001f
#define CHORUS_LFO_OUT_MIN_IN_SECONDS 0.005f
#define CHORUS_LFO_OUT_MAX_IN_SECONDS 0.03f
#define FLANGER_LFO_OUT_MIN_IN_SECONDS 0.001f
#define FLANGER_LFO_OUT_MAX_IN_SECONDS 0.005f

//==============================================================================
/**
 */
class DaalModAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DaalModAudioProcessor();
    ~DaalModAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    //==============================================================================
    float lerp(float x0, float x1, float t); // t = "inPhase"
    
    
private:
    //==============================================================================
    // Circular buffer
    float* _circularBufferLeft;
    float* _circularBufferRight;
    int _circularBufferWriteHead;
    int _circularBufferLength;
    
    // Feedback
    float _feedbackLeft;
    float _feedbackRight;
    
    // LFO
    float _lfoPhase;
    
    // User-controlled parameters
    AudioParameterFloat* _dryWetParam;
    AudioParameterFloat* _depthParam;
    AudioParameterFloat* _rateParam;
    AudioParameterFloat* _phaseOffsetParam;
    AudioParameterFloat* _feedbackParam;
    // AudioParameterFloat* _delayTimeParam; // No longer necessary; we use depth param instead
    
    AudioParameterInt* _typeParam;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalModAudioProcessor)
};

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AllPassFilter.h"
#include "FeedbackDelayNetwork.h"
#include "FractionalDelay.h"
#include "LowPassFilter.h"
#include "SchroederReverb.h"

enum class DaalHallaReverbAlgorithm
{
    kNone=0,
    kSchroeder,
    kStautnerPuckette,
    kNumReverbAlgorithms
};

//==============================================================================
/**
*/
class DaalHallaAudioProcessor  : public AudioProcessor,
                                 public ValueTree::Listener
{
public:
    //==============================================================================
    DaalHallaAudioProcessor();
    ~DaalHallaAudioProcessor();

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
    void valueTreePropertyChanged(ValueTree& treeWhosePropertyhasChanged, const Identifier& property) override;
    
    //==============================================================================
    AudioProcessorValueTreeState& getValueTreeState() { return m_ValueTreeState; }
    

private:
    //==============================================================================
    AudioProcessorValueTreeState m_ValueTreeState;
    bool m_ShouldUpdateParameters = false;
    
    // Predelay
    FractionalDelay m_Predelay;
    
    // Stautner-Puckette
    FeedbackDelayNetwork m_FDN;
    AllPassFilter m_APF1 {240.0f, 0.777f};
    AllPassFilter m_APF2 {82.0f, 0.888f};
    
    // Schroeder
    SchroederReverb m_Schroeder;
    
    // LPF
    LowPassFilter m_LPF;
    
    //==============================================================================
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void updateParameters();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalHallaAudioProcessor)
};

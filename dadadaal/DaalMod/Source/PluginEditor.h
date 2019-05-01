/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
 */
class DaalModAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DaalModAudioProcessorEditor (DaalModAudioProcessor&);
    ~DaalModAudioProcessorEditor();
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalModAudioProcessor& processor;
    
    //==============================================================================
    // UI elements
    Slider _dryWetSlider;
    Slider _depthSlider;
    Slider _rateSlider;
    Slider _phaseOffsetSlider;
    Slider _feedbackSlider;
    // Slider _delayTimeSlider;
    
    ComboBox _typeBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalModAudioProcessorEditor)
};

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../DaalLookAndFeel.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DaalDelAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DaalDelAudioProcessorEditor (DaalDelAudioProcessor&);
    ~DaalDelAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    void setupSlider(Slider& slider, float minVal, float maxVal, float defaultVal);
    
    Slider _delayFeedbackGainSlider;
    Slider _delayTimeMinSlider;
    Slider _delayTimeMaxSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> _delayFeedbackGainValue;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> _delayTimeMinValue;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> _delayTimeMaxValue;
    
    DaalLookAndFeel _daalLookAndFeel;
    
    int _delayFeedbackGainValueOscRate = 1;
    int _delayTimeMinValueOscRate = 2;
    int _delayTimeMaxValueOscRate = 3;
    
    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalDelAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalDelAudioProcessorEditor)
};

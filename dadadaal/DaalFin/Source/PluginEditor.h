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
class DaalFinAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DaalFinAudioProcessorEditor (DaalFinAudioProcessor&);
    ~DaalFinAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Slider filterCutoffDial;
    Slider filterResonanceDial;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterCutoffValue; // Has to come AFTER the UI elements
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> filterResonanceValue;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalFinAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalFinAudioProcessorEditor)
};

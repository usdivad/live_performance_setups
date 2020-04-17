/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DaalSamAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DaalSamAudioProcessorEditor (DaalSamAudioProcessor&);
    ~DaalSamAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    TextButton m_LoadButton {":)"};

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalSamAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalSamAudioProcessorEditor)
};

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
class DaalSamAudioProcessorEditor  : public AudioProcessorEditor,
                                     public FileDragAndDropTarget
{
public:
    DaalSamAudioProcessorEditor (DaalSamAudioProcessor&);
    ~DaalSamAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==============================================================================
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

private:
    //==============================================================================
    TextButton m_LoadButton {":)"};
    std::vector<float> m_AudioPoints;
    bool m_ShouldPaint {false};

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalSamAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalSamAudioProcessorEditor)
};

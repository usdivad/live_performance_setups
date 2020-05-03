/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../DaalLookAndFeel.h"

//==============================================================================
/**
*/
class DaalSamAudioProcessorEditor  : public AudioProcessorEditor,
                                     public FileDragAndDropTarget
{
public:
    DaalSamAudioProcessorEditor (DaalSamAudioProcessor&);
    ~DaalSamAudioProcessorEditor();

    //==============================================    ================================
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
    
    DaalLookAndFeel m_DaalLookAndFeel;
    
    Slider m_AttackSlider;
    Slider m_DecaySlider;
    Slider m_SustainSlider;
    Slider m_ReleaseSlider;
    
    Label m_AttackLabel;
    Label m_DecayLabel;
    Label m_SustainLabel;
    Label m_ReleaseLabel;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_AttackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_DecayAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_SustainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_ReleaseAttachment;
    
    //==============================================================================
    // void setupSlider(Slider& slider, Slider::Style style, float minVal, float maxVal, float defaultVal);

    //==============================================================================
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalSamAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalSamAudioProcessorEditor)
};

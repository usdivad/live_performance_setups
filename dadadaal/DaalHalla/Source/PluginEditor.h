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
class DaalHallaAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    DaalHallaAudioProcessorEditor (DaalHallaAudioProcessor&);
    ~DaalHallaAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DaalHallaAudioProcessor& processor;
    
    //==============================================================================
    
    DaalLookAndFeel m_DaalLookAndFeel;
    
    Label m_TitleLabel;
    
    Slider m_TimeSlider;
    Slider m_ModulationSlider;
    Slider m_DryWetSlider;
    Slider m_PredelaySlider;
    Slider m_DiffusionSlider;
    Slider m_LPFSlider;

    Label m_TimeLabel;
    Label m_ModulationLabel;
    Label m_DryWetLabel;
    Label m_PredelayLabel;
    Label m_DiffusionLabel;
    Label m_LPFLabel;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_TimeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_ModulationAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_DryWetAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_PredelayAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_DiffusionAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> m_LPFAttachment;
    
    //==============================================================================
    void setupSlider(Slider& slider, Label& label, std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& attachment, String paramId, String paramName);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DaalHallaAudioProcessorEditor)
};

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalHallaAudioProcessorEditor::DaalHallaAudioProcessorEditor (DaalHallaAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setupSlider(m_TimeSlider, m_TimeLabel, m_TimeAttachment, "TIME", "Time");
    setupSlider(m_ModulationSlider, m_ModulationLabel, m_ModulationAttachment, "MODULATION", "Modulation");
    setupSlider(m_DryWetSlider, m_DryWetLabel, m_DryWetAttachment, "DRYWET", "Dry/Wet");
    setupSlider(m_PredelaySlider, m_PredelayLabel, m_PredelayAttachment, "PREDELAY", "Predelay");
    setupSlider(m_DiffusionSlider, m_DiffusionLabel, m_DiffusionAttachment, "DIFFUSION", "Diffusion");
    setupSlider(m_LPFSlider, m_LPFLabel, m_LPFAttachment, "LPF", "LPF");
    
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DaalHallaAudioProcessorEditor::~DaalHallaAudioProcessorEditor()
{
}

//==============================================================================
void DaalHallaAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void DaalHallaAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    float sliderX = 0.5f;
    float sliderY = 0.35f;
    float sliderWidth = 0.25f;
    float sliderHeight = 0.25f;
    
    sliderX -= sliderWidth * 0.5f;
    // sliderY -= sliderHeight * 0.5f;
    
    // m_TimeSlider.setBounds(100, 100, 100, 100);
    m_TimeSlider.setBoundsRelative(sliderX * 0.5f, sliderY, sliderWidth, sliderHeight);
    m_ModulationSlider.setBoundsRelative(sliderX, sliderY, sliderWidth, sliderHeight);
    m_DryWetSlider.setBoundsRelative(sliderX * 1.5f, sliderY, sliderWidth, sliderHeight);
    
    m_PredelaySlider.setBoundsRelative(sliderX * 0.5f, sliderY * 2, sliderWidth, sliderHeight);
    m_DiffusionSlider.setBoundsRelative(sliderX, sliderY * 2, sliderWidth, sliderHeight);
    m_LPFSlider.setBoundsRelative(sliderX * 1.5f, sliderY * 2, sliderWidth, sliderHeight);
}

void DaalHallaAudioProcessorEditor::setupSlider(Slider& slider, Label& label, std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& attachment, String paramID, String paramName)
{
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, 40, 20);
    addAndMakeVisible(slider);
    
    label.setText(paramName, NotificationType::dontSendNotification);
    label.setJustificationType(Justification::centredTop);
    label.attachToComponent(&slider, false);
    
    attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), paramID, slider);
}
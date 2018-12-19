/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalFinAudioProcessorEditor::DaalFinAudioProcessorEditor (DaalFinAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);
    
    filterCutoffDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0, 20000.0);
    filterCutoffDial.setValue(600.0);
    filterCutoffDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);
    
    filterResonanceDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterResonanceDial.setRange(1.0, 5.0);
    filterResonanceDial.setValue(1.0);
    filterResonanceDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterResonanceDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResonanceDial);
    
    filterCutoffValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "cutoff", filterCutoffDial);
    filterResonanceValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "resonance", filterResonanceDial);
    
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0);
}

DaalFinAudioProcessorEditor::~DaalFinAudioProcessorEditor()
{
}

//==============================================================================
void DaalFinAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
//
//    g.setColour (Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    
    Rectangle<int> titleArea(0, 10, getWidth(), 20);
    
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawText("DaalFin", titleArea, Justification::centredTop);
    g.drawText("Cutoff", 40, 70, 50, 25, Justification::centredLeft);
    g.drawText("Q", 100, 70, 70, 25, Justification::centredLeft);
    
    Rectangle<float> area(25, 25, 150, 150);
    g.setColour(Colours::royalblue);
    g.drawRoundedRectangle(area, 20.0, 2.0);
}

void DaalFinAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
//    Rectangle<int> area = getLocalBounds().reduced(40);
    
    filterCutoffDial.setBounds(30, 90, 70, 70);
    filterResonanceDial.setBounds(100, 90, 70, 70);
    
}

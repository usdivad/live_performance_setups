/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalGAudioProcessorEditor::DaalGAudioProcessorEditor (DaalGAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // Get parameters from processor
    auto& params = processor.getParameters();
    
    
    // Setup gain slider left
    // (Listener pattern)
    AudioParameterFloat* gainParameterLeft = (AudioParameterFloat*) params.getUnchecked(0);
    _gainControlSliderLeft.setRange(gainParameterLeft->range.start, gainParameterLeft->range.end);
    _gainControlSliderLeft.setValue(*gainParameterLeft);
    _gainControlSliderLeft.setBounds(100,50,100,100);
    _gainControlSliderLeft.setSliderStyle(Slider::SliderStyle::LinearVertical);
    _gainControlSliderLeft.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    // Register as listener
    _gainControlSliderLeft.addListener(this);
    
    addAndMakeVisible(_gainControlSliderLeft);
    
    
    
    // Setup gain slider right
    // (Lambda callback)
    AudioParameterFloat* gainParameterRight = (AudioParameterFloat*) params.getUnchecked(1);
    _gainControlSliderRight.setRange(gainParameterRight->range.start, gainParameterRight->range.end);
    _gainControlSliderRight.setValue(*gainParameterRight);
    _gainControlSliderRight.setBounds(200,50,100,100);
    _gainControlSliderRight.setSliderStyle(Slider::SliderStyle::LinearVertical);
    _gainControlSliderRight.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    // Using lambda capturing
    _gainControlSliderRight.onValueChange = [this, gainParameterRight] {
        *gainParameterRight = _gainControlSliderRight.getValue();
    };
    _gainControlSliderRight.onDragStart = [gainParameterRight] {
        gainParameterRight->beginChangeGesture();
    };
    _gainControlSliderRight.onDragEnd = [gainParameterRight] {
        gainParameterRight->endChangeGesture();
    };
    
    addAndMakeVisible(_gainControlSliderRight);
}

DaalGAudioProcessorEditor::~DaalGAudioProcessorEditor()
{
}

//==============================================================================
void DaalGAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // g.setColour (Colours::white);
    // g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DaalGAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//==============================================================================
void DaalGAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    auto& params = processor.getParameters();
    
    if (slider == &_gainControlSliderLeft) {
        AudioParameterFloat* gainParameterLeft = (AudioParameterFloat*) params.getUnchecked(0);
        *gainParameterLeft = _gainControlSliderLeft.getValue();
        DBG("Gain control left slider value changed to " << _gainControlSliderLeft.getValue());
    }
}

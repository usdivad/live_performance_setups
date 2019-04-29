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
    
    // Setup slider
    auto& params = processor.getParameters();
    AudioParameterFloat* gainParameter = (AudioParameterFloat*) params.getUnchecked(0);
    _gainControlSlider.setRange(gainParameter->range.start, gainParameter->range.end);
    _gainControlSlider.setValue(*gainParameter);
    _gainControlSlider.setBounds(0,0,100,100);
    _gainControlSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _gainControlSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    _gainControlSlider.addListener(this);
    addAndMakeVisible(_gainControlSlider);
}

DaalGAudioProcessorEditor::~DaalGAudioProcessorEditor()
{
}

//==============================================================================
void DaalGAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DaalGAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

//==============================================================================
void DaalGAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    auto& params = processor.getParameters();
    
    if (slider == &_gainControlSlider) {
        AudioParameterFloat* gainParameter = (AudioParameterFloat*) params.getUnchecked(0);
        *gainParameter = _gainControlSlider.getValue();
        DBG("Gain control slider value changed to " << _gainControlSlider.getValue());
    }
}

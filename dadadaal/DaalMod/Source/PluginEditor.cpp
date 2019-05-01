/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalModAudioProcessorEditor::DaalModAudioProcessorEditor (DaalModAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // Get params from processor
    auto& params = processor.getParameters();
    AudioParameterFloat* dryWetParam = (AudioParameterFloat*)params.getUnchecked(0);
    AudioParameterFloat* depthParam = (AudioParameterFloat*)params.getUnchecked(1);
    AudioParameterFloat* rateParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterFloat* phaseOffsetParam = (AudioParameterFloat*)params.getUnchecked(3);
    AudioParameterFloat* feedbackParam = (AudioParameterFloat*)params.getUnchecked(4);
    // AudioParameterFloat* delayTimeParam = (AudioParameterFloat*)params.getUnchecked(2);
    AudioParameterInt* typeParam = (AudioParameterInt*)params.getUnchecked(5);
    
    // Dry/Wet
    _dryWetSlider.setBounds(0, 0, 100, 100);
    _dryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _dryWetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _dryWetSlider.setRange(dryWetParam->range.start, dryWetParam->range.end);
    _dryWetSlider.setValue(dryWetParam->get());
    addAndMakeVisible(_dryWetSlider);
    
    _dryWetSlider.onValueChange = [this, dryWetParam] {
        *dryWetParam = _dryWetSlider.getValue();
    };
    _dryWetSlider.onDragStart = [dryWetParam] {
        dryWetParam->beginChangeGesture();
    };
    _dryWetSlider.onDragEnd = [dryWetParam] {
        dryWetParam->endChangeGesture();
    };
    
    // Depth
    _depthSlider.setBounds(100, 0, 100, 100);
    _depthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _depthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _depthSlider.setRange(depthParam->range.start, depthParam->range.end);
    _depthSlider.setValue(depthParam->get());
    addAndMakeVisible(_depthSlider);
    
    _depthSlider.onValueChange = [this, depthParam] {
        *depthParam = _depthSlider.getValue();
    };
    _depthSlider.onDragStart = [depthParam] {
        depthParam->beginChangeGesture();
    };
    _depthSlider.onDragEnd = [depthParam] {
        depthParam->endChangeGesture();
    };
    
    
    // Rate
    _rateSlider.setBounds(200, 0, 100, 100);
    _rateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _rateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _rateSlider.setRange(rateParam->range.start, rateParam->range.end);
    _rateSlider.setValue(rateParam->get());
    addAndMakeVisible(_rateSlider);
    
    _rateSlider.onValueChange = [this, rateParam] {
        *rateParam = _rateSlider.getValue();
    };
    _rateSlider.onDragStart = [rateParam] {
        rateParam->beginChangeGesture();
    };
    _rateSlider.onDragEnd = [rateParam] {
        rateParam->endChangeGesture();
    };
    
    // Phase Offset
    _phaseOffsetSlider.setBounds(300, 0, 100, 100);
    _phaseOffsetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _phaseOffsetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _phaseOffsetSlider.setRange(phaseOffsetParam->range.start, phaseOffsetParam->range.end);
    _phaseOffsetSlider.setValue(phaseOffsetParam->get());
    addAndMakeVisible(_phaseOffsetSlider);
    
    _phaseOffsetSlider.onValueChange = [this, phaseOffsetParam] {
        *phaseOffsetParam = _phaseOffsetSlider.getValue();
    };
    _phaseOffsetSlider.onDragStart = [phaseOffsetParam] {
        phaseOffsetParam->beginChangeGesture();
    };
    _phaseOffsetSlider.onDragEnd = [phaseOffsetParam] {
        phaseOffsetParam->endChangeGesture();
    };
    
    // Feedback
    _feedbackSlider.setBounds(0, 100, 100, 100);
    _feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    _feedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    _feedbackSlider.setRange(feedbackParam->range.start, feedbackParam->range.end);
    _feedbackSlider.setValue(feedbackParam->get());
    addAndMakeVisible(_feedbackSlider);
    
    _feedbackSlider.onValueChange = [this, feedbackParam] {
        *feedbackParam = _feedbackSlider.getValue();
    };
    _feedbackSlider.onDragStart = [feedbackParam] {
        feedbackParam->beginChangeGesture();
    };
    _feedbackSlider.onDragEnd = [feedbackParam] {
        feedbackParam->endChangeGesture();
    };
    
    // Delay time
    // _delayTimeSlider.setBounds(200, 0, 100, 100);
    // _delayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    // _delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    // _delayTimeSlider.setRange(delayTimeParam->range.start, delayTimeParam->range.end);
    // _delayTimeSlider.setValue(delayTimeParam->get());
    // addAndMakeVisible(_delayTimeSlider);
    //
    // _delayTimeSlider.onValueChange = [this, delayTimeParam] {
    //     *delayTimeParam = _delayTimeSlider.getValue();
    // };
    // _delayTimeSlider.onDragStart = [delayTimeParam] {
    //     delayTimeParam->beginChangeGesture();
    // };
    // _delayTimeSlider.onDragEnd = [delayTimeParam] {
    //     delayTimeParam->endChangeGesture();
    // };
    
    
    // Type box (chorus vs. flanger)
    _typeBox.setBounds(100, 100, 100, 30);
    _typeBox.addItem("Chorus", 1);
    _typeBox.addItem("Flanger", 2);
    addAndMakeVisible(_typeBox);
    
    _typeBox.onChange = [this, typeParam] {
        typeParam->beginChangeGesture();
        *typeParam = _typeBox.getSelectedItemIndex();
        typeParam->endChangeGesture();
    };
    _typeBox.setSelectedItemIndex(typeParam->get()); // Show currently selected
}

DaalModAudioProcessorEditor::~DaalModAudioProcessorEditor()
{
}

//==============================================================================
void DaalModAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // g.setColour (Colours::white);
    // g.setFont (15.0f);
    // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DaalModAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

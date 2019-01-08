/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalDelAudioProcessorEditor::DaalDelAudioProcessorEditor (DaalDelAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setLookAndFeel(&_daalLookAndFeel);
    
    // Setup sliders
    setupSlider(_delayFeedbackGainSlider, 0.0f, 1.0f, 0.5f);
    setupSlider(_delayTimeMinSlider, 0.0f, 20000.0f, 1000.0f);
    setupSlider(_delayTimeMaxSlider, 0.0f, 20000.0f, 2000.0f);
    
    // Add attachments
    _delayFeedbackGainValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "delayFeedbackGain", _delayFeedbackGainSlider);
    _delayTimeMinValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "delayTimeMin", _delayTimeMinSlider);
    _delayTimeMaxValue = new AudioProcessorValueTreeState::SliderAttachment(processor.tree, "delayTimeMax", _delayTimeMaxSlider);
}

DaalDelAudioProcessorEditor::~DaalDelAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    _delayFeedbackGainSlider.setLookAndFeel(nullptr);
    _delayTimeMinSlider.setLookAndFeel(nullptr);
    _delayTimeMaxSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DaalDelAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

//    g.setColour (Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
    
    g.setColour(getLookAndFeel().findColour(Label::textColourId));
    
    g.setColour(Colour(0xff666666));
    g.drawText("Delay Time", (getWidth()/2) - (150/2) - 5, 20, 150, 50, Justification::centred);
    g.drawText("Delay Time", (getWidth()/2) - (150/2) + 5, 20, 150, 50, Justification::centred);
    
    g.setColour(getLookAndFeel().findColour(Label::textColourId));
    g.drawText("Delay Time", (getWidth()/2) - (200/2), 20, 200, 50, Justification::centred);
    g.drawText("Min", 50, 100, 200, 50, Justification::centred);
    g.drawText("Max", 150, 100, 200, 50, Justification::centred);
    g.drawText("Feedback", (getWidth()/2) - (200/2), 200, 200, 50, Justification::centred);
}

void DaalDelAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int windowWidth = getWidth();
    int windowHeight = getHeight();
    int sliderWidth = 75;
    int delayTimeSlidersCenterXOffset = 50;
    int delayTimeSlidersY = 50;
    // int delayFeedbackGainSliderY = (windowHeight/2) - (sliderWidth/2);
    int delayFeedbackGainSliderY = 150;
    
    _delayFeedbackGainSlider.setBounds((windowWidth/2) - (sliderWidth/2), delayFeedbackGainSliderY, sliderWidth, sliderWidth);
    _delayTimeMinSlider.setBounds((windowWidth/2) - delayTimeSlidersCenterXOffset - (sliderWidth/2), delayTimeSlidersY, sliderWidth, sliderWidth);
    _delayTimeMaxSlider.setBounds((windowWidth/2) + delayTimeSlidersCenterXOffset - (sliderWidth/2), delayTimeSlidersY, sliderWidth, sliderWidth);
}

//==============================================================================
void DaalDelAudioProcessorEditor::setupSlider(Slider& slider, float minVal, float maxVal, float defaultVal) {
    slider.setRange(minVal, maxVal);
    slider.setValue(defaultVal);
    
    slider.setLookAndFeel(&_daalLookAndFeel);
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    // slider.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&slider);
}

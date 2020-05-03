/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalSamAudioProcessorEditor::DaalSamAudioProcessorEditor (DaalSamAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    
    // Load button
    m_LoadButton.onClick = [&]()
    {
        m_ShouldPaint = true;
        processor.loadFile();
        repaint();
    };
    
    addAndMakeVisible(m_LoadButton);
    
    
    // Sliders
    // TODO: Use setupSlider()
    
    Slider::SliderStyle sliderStyle = Slider::SliderStyle::RotaryVerticalDrag;
    Slider::TextEntryBoxPosition sliderTextBoxStyle = Slider::TextBoxBelow;
    bool sliderTextBoxReadOnly = true;
    int sliderTextBoxWidth = 45;
    int sliderTextBoxHeight = 25;
    float sliderRangeMin = 0.0f;
    float sliderRangeMax = 5.0f;
    float sliderRangeStep = 0.01f;
    
    m_AttackSlider.setSliderStyle(sliderStyle);
    m_AttackSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    m_AttackSlider.setRange(sliderRangeMin, sliderRangeMax, sliderRangeStep);
    m_AttackSlider.addListener(this);
    addAndMakeVisible(m_AttackSlider);
    
    m_AttackLabel.setText("A", NotificationType::dontSendNotification);
    m_AttackLabel.setJustificationType(Justification::centredTop);
    m_AttackLabel.attachToComponent(&m_AttackSlider, false);
    
    m_DecaySlider.setSliderStyle(sliderStyle);
    m_DecaySlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    m_DecaySlider.setRange(sliderRangeMin, sliderRangeMax, sliderRangeStep);
    m_DecaySlider.addListener(this);
    addAndMakeVisible(m_DecaySlider);
    
    m_DecayLabel.setText("D", NotificationType::dontSendNotification);
    m_DecayLabel.setJustificationType(Justification::centredTop);
    m_DecayLabel.attachToComponent(&m_DecaySlider, false);
    
    m_SustainSlider.setSliderStyle(sliderStyle);
    m_SustainSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    m_SustainSlider.setRange(sliderRangeMin, sliderRangeMax, sliderRangeStep);
    m_SustainSlider.addListener(this);
    addAndMakeVisible(m_SustainSlider);
    
    m_SustainLabel.setText("S", NotificationType::dontSendNotification);
    m_SustainLabel.setJustificationType(Justification::centredTop);
    m_SustainLabel.attachToComponent(&m_SustainSlider, false);
    
    m_ReleaseSlider.setSliderStyle(sliderStyle);
    m_ReleaseSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    m_ReleaseSlider.setRange(sliderRangeMin, sliderRangeMax, sliderRangeStep);
    m_ReleaseSlider.addListener(this);
    addAndMakeVisible(m_ReleaseSlider);
    
    m_ReleaseLabel.setText("R", NotificationType::dontSendNotification);
    m_ReleaseLabel.setJustificationType(Justification::centredTop);
    m_ReleaseLabel.attachToComponent(&m_ReleaseSlider, false);
    
    
    // Do this *after* setting up the individual components
    setSize (400, 300);
    setLookAndFeel(&m_DaalLookAndFeel);
}

DaalSamAudioProcessorEditor::~DaalSamAudioProcessorEditor()
{
}

//==============================================================================
void DaalSamAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
    
    // ========
    // Sample load status
    if (processor.getNumSamplerSounds() > 0)
    {
        g.setColour (Colours::green);
        g.drawText(":D", getWidth() / 4 - 50, getHeight() / 4 - 50, 100, 100, Justification::centred);
    }
    else
    {
        g.setColour (Colours::red);
        g.drawText("D:", getWidth() / 4 - 50, getHeight() / 4 - 50, 100, 100, Justification::centred);
    }
    
    // This condition helps efficiency but causes waveform to disappear when clicking in the plugin window
    // if (m_ShouldPaint)
    // {
        // ========
        // Waveform drawing
        
        Path p;
        m_AudioPoints.clear();
        
        auto waveform = processor.getWaveform();
        auto ratioX = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        // Scaling x axis
        for (int i=0; i<waveform.getNumSamples(); i+=ratioX)
        {
            m_AudioPoints.push_back(buffer[i]);
        }
        
        // Start path
        g.setColour(Colours::white);
        p.startNewSubPath(0, getHeight()/2);
        
        // Scaling y axis and connecting path
        for (int i=0; i<m_AudioPoints.size(); i++)
        {
            auto point = jmap<float>(m_AudioPoints[i], -1, 1, getHeight(), 0);
            p.lineTo(i, point);
        }
        
        g.strokePath(p, PathStrokeType(2));
        m_ShouldPaint = false;
    // }
    

}

void DaalSamAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    m_LoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 100, 100, 100);
    
    float sliderX = 0.0f;
    float sliderY = 0.65f;
    float sliderWidth = 0.25f;
    float sliderHeight = 0.3f;
    
    m_AttackSlider.setBoundsRelative(sliderX, sliderY, sliderWidth, sliderHeight);
    m_DecaySlider.setBoundsRelative(sliderX + (sliderWidth), sliderY, sliderWidth, sliderHeight);
    m_SustainSlider.setBoundsRelative(sliderX + (sliderWidth * 2), sliderY, sliderWidth, sliderHeight);
    m_ReleaseSlider.setBoundsRelative(sliderX + (sliderWidth * 3), sliderY, sliderWidth, sliderHeight);

}

//==============================================================================

bool DaalSamAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".aif") || file.contains(".mp3") || file.contains(".m4a"))
        {
            return true;
        }
            
    }
    
    return false;
}

void DaalSamAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            m_ShouldPaint = true;
            processor.loadFile(file);
        }
    }
    
    repaint();
}

void DaalSamAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &m_AttackSlider)
    {
        processor.getADSRParams().attack = m_AttackSlider.getValue();
    }
    else if (slider == &m_DecaySlider)
    {
        processor.getADSRParams().decay = m_DecaySlider.getValue();
    }
    else if (slider == &m_SustainSlider)
    {
        processor.getADSRParams().sustain = m_SustainSlider.getValue();
    }
    else if (slider == &m_ReleaseSlider)
    {
        processor.getADSRParams().release = m_ReleaseSlider.getValue();
    }
    
    processor.updateADSR();
}

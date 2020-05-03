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
    // float sliderRangeMin = 0.0f;
    // float sliderRangeMax = 5.0f;
    float sliderRangeStep = 0.01f;
    
    // Attack
    
    m_AttackSlider.setSliderStyle(sliderStyle);
    m_AttackSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    addAndMakeVisible(m_AttackSlider);
    
    m_AttackLabel.setText("A", NotificationType::dontSendNotification);
    m_AttackLabel.setJustificationType(Justification::centredTop);
    m_AttackLabel.attachToComponent(&m_AttackSlider, false);
    
    m_AttackAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "ATTACK", m_AttackSlider);
    
    // Decay
    
    m_DecaySlider.setSliderStyle(sliderStyle);
    m_DecaySlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    addAndMakeVisible(m_DecaySlider);
    
    m_DecayLabel.setText("D", NotificationType::dontSendNotification);
    m_DecayLabel.setJustificationType(Justification::centredTop);
    m_DecayLabel.attachToComponent(&m_DecaySlider, false);
    
    m_DecayAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "DECAY", m_DecaySlider);

    
    // Sustain
    
    m_SustainSlider.setSliderStyle(sliderStyle);
    m_SustainSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    addAndMakeVisible(m_SustainSlider);
    
    m_SustainLabel.setText("S", NotificationType::dontSendNotification);
    m_SustainLabel.setJustificationType(Justification::centredTop);
    m_SustainLabel.attachToComponent(&m_SustainSlider, false);
    
    m_SustainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "SUSTAIN", m_SustainSlider);

    
    // Release
    
    m_ReleaseSlider.setSliderStyle(sliderStyle);
    m_ReleaseSlider.setTextBoxStyle(sliderTextBoxStyle, sliderTextBoxReadOnly, sliderTextBoxWidth, sliderTextBoxHeight);
    addAndMakeVisible(m_ReleaseSlider);
    
    m_ReleaseLabel.setText("R", NotificationType::dontSendNotification);
    m_ReleaseLabel.setJustificationType(Justification::centredTop);
    m_ReleaseLabel.attachToComponent(&m_ReleaseSlider, false);
    
    m_ReleaseAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "RELEASE", m_ReleaseSlider);

    
    
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

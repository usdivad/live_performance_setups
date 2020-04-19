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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    
    m_LoadButton.onClick = [&]()
    {
        processor.loadFile();
    };
    
    addAndMakeVisible(m_LoadButton);
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
    
    m_LoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
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

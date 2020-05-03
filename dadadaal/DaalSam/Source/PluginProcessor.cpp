/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalSamAudioProcessor::DaalSamAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    for (int i=0; i<m_NumVoices; i++)
    {
        m_Sampler.addVoice(new SamplerVoice());
    }
    
    m_FormatManager.registerBasicFormats();
}

DaalSamAudioProcessor::~DaalSamAudioProcessor()
{
    m_FormatReader = nullptr;
}

//==============================================================================
const String DaalSamAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalSamAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalSamAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalSamAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalSamAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalSamAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalSamAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalSamAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalSamAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalSamAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalSamAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    m_Sampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
}

void DaalSamAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalSamAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DaalSamAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    
    m_Sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool DaalSamAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalSamAudioProcessor::createEditor()
{
    return new DaalSamAudioProcessorEditor (*this);
}

//==============================================================================
void DaalSamAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalSamAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

void DaalSamAudioProcessor::loadFile()
{
    m_Sampler.clearSounds();

    FileChooser chooser {":)"};
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        m_FormatReader = m_FormatManager.createReaderFor(file);
    }
    
    BigInteger midiRange;
    midiRange.setRange(0, 128, true);
    
    m_Sampler.addSound(new SamplerSound("Sample", *m_FormatReader, midiRange, 60, 0.1, 0.1, 10));
}

void DaalSamAudioProcessor::loadFile(const String& path)
{
    m_Sampler.clearSounds();
    
    auto file = File(path);
    m_FormatReader = m_FormatManager.createReaderFor(file);
    
    int sampleLength = static_cast<int>(m_FormatReader->lengthInSamples);
    m_Waveform.setSize(1, sampleLength);
    m_FormatReader->read(&m_Waveform, 0, sampleLength, 0, true, true);
    
    BigInteger midiRange;
    midiRange.setRange(0, 128, true);
    
    m_Sampler.addSound(new SamplerSound("Sample", *m_FormatReader, midiRange, 60, 0.1, 0.1, 10));
}

void DaalSamAudioProcessor::updateADSR()
{
    for (int i=0; i<m_Sampler.getNumSounds(); i++)
    {
        if (auto sound = dynamic_cast<SamplerSound*>(m_Sampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(m_ADSRParams);
        }
        
    }
    
    DBG("A=" << m_ADSRParams.attack << ", D=" << m_ADSRParams.decay << ", S=" << m_ADSRParams.sustain << ", R=" << m_ADSRParams.release);
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalSamAudioProcessor();
}

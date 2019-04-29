/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalGAudioProcessor::DaalGAudioProcessor()
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
    addParameter(_gainParameter = new AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));
    _gainSmoothed = _gainParameter->get();
}

DaalGAudioProcessor::~DaalGAudioProcessor()
{
}

//==============================================================================
const String DaalGAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalGAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalGAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalGAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalGAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalGAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalGAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalGAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalGAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalGAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalGAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DaalGAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalGAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DaalGAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    // Adjusting each channel one by one
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        
        for (int sampleIdx = 0; sampleIdx < buffer.getNumSamples(); sampleIdx++) {
            channelData[sampleIdx] *= _gainParameter->get();
        }
    }
     */
    
    // Adjusting left and right channels together
    // TODO: Deal with > 2 channels
    if (buffer.getNumChannels() >= 2) {
        float* channelLeft = buffer.getWritePointer(0);
        float* channelRight = buffer.getWritePointer(1);
        
        // DBG(*_gainParameter); // Don't print from audio thread
        
        for (int sampleIdx = 0; sampleIdx < buffer.getNumSamples(); sampleIdx++) {
            _gainSmoothed = _gainSmoothed - (0.005 * (_gainSmoothed - _gainParameter->get()));
            
            channelLeft[sampleIdx] *= _gainSmoothed;
            channelRight[sampleIdx] *= _gainSmoothed;
        }
    }
}

//==============================================================================
bool DaalGAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalGAudioProcessor::createEditor()
{
    return new DaalGAudioProcessorEditor (*this);
}

//==============================================================================
void DaalGAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalGAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalGAudioProcessor();
}

/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalFinAudioProcessor::DaalFinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
//        tree(*this, nullptr),
        tree(*this, nullptr, "Parameters", createParameterLayout()),
        lpf(dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0, 0.1))
#endif
{
    // Deprecated
//    NormalisableRange<float> cutoffRange(20.0, 20000.0);
//    NormalisableRange<float> resonanceRange(1.0, 5.0);
//    NormalisableRange<float> filterMenuRange(0.0, 2.0);
    
//    AudioParameterFloat cutoffParam("cutoff", "Cutoff", cutoffRange, 600.0);
//    AudioParameterFloat resonanceParam("resonance", "Resonance", resonanceRange, 1.0);
//    AudioParameterFloat filterMenuParam("filterMenu", "FilterMenu", filterMenuRange, 0.0);
    
//    tree.createAndAddParameter(std::unique_ptr<AudioParameterFloat>(&cutoffParam));
//    tree.createAndAddParameter(std::unique_ptr<AudioParameterFloat>(&resonanceParam));
//    tree.createAndAddParameter(std::unique_ptr<AudioParameterFloat>(&filterMenuParam));
    
//    tree.createAndAddParameter(std::unique_ptr<AudioParameterFloat>("resonance", "Resonance", resonanceRange, 1.0));
//    tree.createAndAddParameter(std::unique_ptr<AudioParameterFloat>("filterMenu", "FilterMenu", filterMenuRange, 0.0));
    
    
    // Even more deprecated
//    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0, nullptr, nullptr);
//    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0, nullptr, nullptr);
    
}

DaalFinAudioProcessor::~DaalFinAudioProcessor()
{
}

//==============================================================================
const String DaalFinAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalFinAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalFinAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalFinAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalFinAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalFinAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalFinAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalFinAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalFinAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalFinAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalFinAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    
    // DSP spec
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    lpf.prepare(spec); // Pass spec in
    lpf.reset(); // Flush garbage
}

void DaalFinAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalFinAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DaalFinAudioProcessor::updateFilter() {
    float frequency = *tree.getRawParameterValue("cutoff");
    float resonance = *tree.getRawParameterValue("resonance");
    *lpf.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, frequency, resonance);
}

void DaalFinAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // DSP
    dsp::AudioBlock<float> block(buffer);
    updateFilter();
    lpf.process(dsp::ProcessContextReplacing<float>(block));
}

//==============================================================================
bool DaalFinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalFinAudioProcessor::createEditor()
{
    return new DaalFinAudioProcessorEditor (*this);
}

//==============================================================================
void DaalFinAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalFinAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalFinAudioProcessor();
}


//==============================================================================
AudioProcessorValueTreeState::ParameterLayout DaalFinAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<AudioParameterFloat>> params;
    
    // Using temp variables for clarity (crs)
//    NormalisableRange<float> cutoffRange(20.0, 20000.0);
//    NormalisableRange<float> resonanceRange(1.0, 5.0);
//
//    AudioParameterFloat cutoffParam("cutoff", "Cutoff", cutoffRange, 600.0);
//    AudioParameterFloat resonanceParam("resonance", "Resonance", resonanceRange, 1.0);
//
//    params.push_back(std::unique_ptr<AudioParameterFloat>(&cutoffParam));
//    params.push_back(std::unique_ptr<AudioParameterFloat>(&resonanceParam));
    
    // Constructing arguments directly
    params.push_back(std::make_unique<AudioParameterFloat>("cutoff", "Cutoff", NormalisableRange<float> (20.0, 20000.0), 600.0));
    params.push_back(std::make_unique<AudioParameterFloat>("resonance", "Resonance", NormalisableRange<float> (1.0, 5.0), 1.0));

    
    return {params.begin(), params.end()};
    
}

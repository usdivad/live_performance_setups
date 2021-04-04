/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalHallaAudioProcessor::DaalHallaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        m_ValueTreeState(*this, nullptr, "Parameters", createParameterLayout())

#endif
{
    m_ValueTreeState.state.addListener(this);
}

DaalHallaAudioProcessor::~DaalHallaAudioProcessor()
{
}

//==============================================================================
const String DaalHallaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalHallaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalHallaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalHallaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalHallaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalHallaAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalHallaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalHallaAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalHallaAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalHallaAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalHallaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Predelay
    m_Predelay.setFs(sampleRate);
    m_Predelay.setDelaySamples(0.0f);
    
    // Stautner-Puckette
    m_FDN.setFs(sampleRate);
    m_APF1.setFs(sampleRate);
    m_APF2.setFs(sampleRate);
    
    // Schroeder
    m_Schroeder.setFs(sampleRate);
    
    // LPF
    m_LPF.setFs(sampleRate);
}

void DaalHallaAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalHallaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DaalHallaAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    
    float predelayDelayAmountInSamples = m_ValueTreeState.getRawParameterValue("PREDELAY")->load() * 0.001f * getSampleRate();
    float dryWet = m_ValueTreeState.getRawParameterValue("DRYWET")->load();
    float verbTime = m_ValueTreeState.getRawParameterValue("TIME")->load();
    float verbModAmt = m_ValueTreeState.getRawParameterValue("MODULATION")->load();
    float diffusionAmt = m_ValueTreeState.getRawParameterValue("DIFFUSION")->load();
    float lpfFreq = m_ValueTreeState.getRawParameterValue("LPF")->load();
    DBG(lpfFreq);
    DaalHallaReverbAlgorithm algorithm = (DaalHallaReverbAlgorithm)m_ValueTreeState.getRawParameterValue("ALGORITHM")->load();
    
    // Predelay
    m_Predelay.setSpeed(0.0f);
    m_Predelay.setDepth(0.0f);
    m_Predelay.setDelaySamples(predelayDelayAmountInSamples);

    // Stautner-Puckette
    m_FDN.setTime(verbTime);
    m_FDN.setModulation(verbModAmt);
    m_APF1.setModulation(verbModAmt);
    m_APF2.setModulation(verbModAmt);
    m_APF1.setFeedbackGain(diffusionAmt);
    m_APF2.setFeedbackGain(diffusionAmt);
    
    // Schroeder
    m_Schroeder.setFeedbackGain(verbTime);
    m_Schroeder.setDiffusionGain(diffusionAmt);
    m_Schroeder.setModulation(verbModAmt);
    
    // LPF
    m_LPF.setFrequency(lpfFreq);

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
        
        for (int n=0; n<buffer.getNumSamples(); n++)
        {
            // Input
            // x = in, w = verb, y = out
            float x = channelData[n];
            
            // Predelay
            float w = m_Predelay.processSample(x, channel);
            
            // Process reverb based on chosen algorithm param
            switch (algorithm)
            {
                // Schroeder
                case (DaalHallaReverbAlgorithm::kSchroeder):
                {
                    w = m_Schroeder.processSample(w, channel);
                } break;
                    
                // Stautner-Puckette
                case (DaalHallaReverbAlgorithm::kStautnerPuckette):
                {
                    w = m_FDN.processSample(w, channel);
                    w = m_APF1.processSample(w, channel); // Do APF in series
                    w = m_APF2.processSample(w, channel);
                } break;
                    
                default:
                {
                    
                } break;
            }
            
            // LPF
            w = m_LPF.processSample(w, channel);
            
            // Output
            float y = ((1.0f - dryWet) * x) + (dryWet * w);
            channelData[n] = y;
        }
    }
    
    
    if (m_ShouldUpdateParameters)
    {
        updateParameters();
        m_ShouldUpdateParameters = false;
    }
}

//==============================================================================
bool DaalHallaAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalHallaAudioProcessor::createEditor()
{
    return new DaalHallaAudioProcessorEditor (*this);
}

//==============================================================================
void DaalHallaAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalHallaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout DaalHallaAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
    
    parameters.push_back(std::make_unique<AudioParameterFloat>("TIME", "Time", 0.4f, 0.7f, 0.5f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("MODULATION", "Modulation", 1.0f, 10.0f, 2.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("DRYWET", "Dry/Wet", 0.0f, 1.0f, 0.5f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("PREDELAY", "Predelay", 0.0f, 200.0f, 0.0f)); // Milliseconds
    parameters.push_back(std::make_unique<AudioParameterFloat>("DIFFUSION", "Diffusion", 0.2f, 0.8f, 0.5f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("LPF", "LPF", 1000.0f, 20000.0f, 10000.0f)); // Hz
    parameters.push_back(std::make_unique<AudioParameterInt>("ALGORITHM", "Reverb Algorithm", (int)DaalHallaReverbAlgorithm::kNone + 1, (int)DaalHallaReverbAlgorithm::kNumReverbAlgorithms - 1, (int)DaalHallaReverbAlgorithm::kSchroeder));
    
    return {parameters.begin(), parameters.end() };
}

void DaalHallaAudioProcessor::valueTreePropertyChanged(ValueTree &treeWhosePropertyhasChanged, const Identifier &property)
{
    m_ShouldUpdateParameters = true;
}

void DaalHallaAudioProcessor::updateParameters()
{
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalHallaAudioProcessor();
}

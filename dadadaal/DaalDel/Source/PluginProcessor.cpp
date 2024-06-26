/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalDelAudioProcessor::DaalDelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        tree(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

DaalDelAudioProcessor::~DaalDelAudioProcessor()
{
}

//==============================================================================
const String DaalDelAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalDelAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DaalDelAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DaalDelAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DaalDelAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalDelAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DaalDelAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalDelAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalDelAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalDelAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalDelAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    const int numInputChannels = getTotalNumInputChannels();
    const int delayBufferSize = 20 * (sampleRate + samplesPerBlock); //  20 seconds (plus a bit)
    
    _delayBuffer.setSize(numInputChannels, delayBufferSize);
    _sampleRate = sampleRate;
}

void DaalDelAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalDelAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DaalDelAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    
    // ====
    // Lengths for circular buffer
    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = _delayBuffer.getNumSamples();
    
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        
        // Set up circular buffer
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = _delayBuffer.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);
        
        // Apply gains (now do this before getting from delay)
        applyDryWetToBuffer(buffer, channel, bufferLength, dryBuffer);
        
        // Copy data from main to delay buffer
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        
        // Copy data from delay buffer to output buffer
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        
        // Feedback
        feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);
    }
    
    _writePosition += bufferLength; // Increment
    _writePosition %= delayBufferLength; // Wrap around position index
    
    // Update values from tree
    updateTreeParams();
}

void DaalDelAudioProcessor::fillDelayBuffer(const int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData) {
    
    if (delayBufferLength > bufferLength + _writePosition) { // Straight copy
        _delayBuffer.copyFromWithRamp(channel, _writePosition, bufferData, bufferLength, _delayFeedbackGain, _delayFeedbackGain);
    }
    else { // Copy with wrap-around
        const int bufferRemaining = delayBufferLength - _writePosition;
        _delayBuffer.copyFromWithRamp(channel, _writePosition, bufferData, bufferRemaining, _delayFeedbackGain, _delayFeedbackGain);
        _delayBuffer.copyFromWithRamp(channel, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, _delayFeedbackGain, _delayFeedbackGain);
    }
}

void DaalDelAudioProcessor::getFromDelayBuffer(AudioBuffer<float>& buffer, const int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData) {
    
    // Set delay time
    int curDelayTime = _curDelayTime;
    
    // Randomize delay time (half vs. full)
    // int dice = rand() % 100;
    // if (dice > 50) {
    //    curDelayTime = static_cast<int>(_delayTimeMax / 2);
    // }
    
    // Randomize delay time (within min to max range)
    curDelayTime = rand() % (std::max(_delayTimeMax - _delayTimeMin, 1));
    curDelayTime += _delayTimeMin;
    
    
    // Just set it to min
    // curDelayTime = _delayTimeMin;
    
    // ====
    
    // Poor man's oscillator (just for testing real quick)
    // TODO: Use a real oscillator
    if (_curDelayTimeDirectionIsUp) { // Up
        curDelayTime++;
        if (curDelayTime >= _delayTimeMax) _curDelayTimeDirectionIsUp = false;
    }
    else { // Down
        curDelayTime--;
        if (curDelayTime <= _delayTimeMin) _curDelayTimeDirectionIsUp = true;
    }
    
    // ====
    
    // Update delay time
    _curDelayTime = curDelayTime;
    DBG("curDelayTime=" + String(curDelayTime));
    
    // Get
    const int readPosition = static_cast<int> (delayBufferLength + _writePosition - (_sampleRate * curDelayTime / 1000)) % delayBufferLength;
    
    if (delayBufferLength > bufferLength + readPosition) { // Straight copy
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferLength, 1 - _delayDryWetGain);
    }
    else { // Copy with wrap-around
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.addFrom(channel, 0, delayBufferData + readPosition, bufferRemaining,  1 - _delayDryWetGain);
        buffer.addFrom(channel, bufferRemaining, delayBufferData, bufferLength - bufferRemaining,  1 - _delayDryWetGain);
    }
}


void DaalDelAudioProcessor::feedbackDelay(const int channel, const int bufferLength, const int delayBufferLength, float* dryBuffer) {
    // Add main signal to the delay signal
    if (delayBufferLength > bufferLength + _writePosition) {
        _delayBuffer.addFromWithRamp(channel, _writePosition, dryBuffer, bufferLength, _delayFeedbackGain, _delayFeedbackGain);
    }
    else {
        const int bufferRemaining = delayBufferLength - _writePosition;
        _delayBuffer.addFromWithRamp(channel, _writePosition, dryBuffer, bufferRemaining, _delayFeedbackGain, _delayFeedbackGain);
        _delayBuffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength - bufferRemaining,  _delayFeedbackGain, _delayFeedbackGain);
    }
}

void DaalDelAudioProcessor::applyDryWetToBuffer(AudioBuffer<float>& buffer, const int channel, const int bufferLength, float* dryBuffer) {
    // buffer.addFromWithRamp(channel, 0, dryBuffer, bufferLength, _delayDryWetGain, _delayDryWetGain); // If we're adding after delay
    buffer.applyGain(channel, 0, bufferLength, _delayDryWetGain);
}

//==============================================================================
bool DaalDelAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalDelAudioProcessor::createEditor()
{
    return new DaalDelAudioProcessorEditor (*this);
}

//==============================================================================
void DaalDelAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalDelAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalDelAudioProcessor();
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout DaalDelAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<AudioParameterFloat>> params;
    
    params.push_back(std::make_unique<AudioParameterFloat>("delayFeedbackGain", "Delay Feedback Gain", NormalisableRange<float> (0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("delayTimeMin", "Delay Time Min", NormalisableRange<float> (0.0f, 10000.0f), 1000.0f));
        params.push_back(std::make_unique<AudioParameterFloat>("delayTimeMax", "Delay Time Max", NormalisableRange<float> (0.0f, 10000.0f), 2000.0f));
    
    return {params.begin(), params.end()};
}

void DaalDelAudioProcessor::updateTreeParams()
{
    _delayFeedbackGain = *tree.getRawParameterValue("delayFeedbackGain");
    _delayTimeMax = *tree.getRawParameterValue("delayTimeMax");
    _delayTimeMin = *tree.getRawParameterValue("delayTimeMin");
}

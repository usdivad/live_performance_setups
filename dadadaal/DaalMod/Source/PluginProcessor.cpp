/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin processor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DaalModAudioProcessor::DaalModAudioProcessor()
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
    // Circular buffers are set to null pointers because we don't know sample rate yet, and thus don't know how to instantiate the audio data. These will be set in prepareToPlay()
    _circularBufferLeft = nullptr;
    _circularBufferRight = nullptr;
    _circularBufferWriteHead = 0;
    _circularBufferLength = 0;
    
    _feedbackLeft = 0;
    _feedbackRight = 0;
    
    _lfoPhase = 0;
    
    // Parameters
    addParameter(_dryWetParam = new AudioParameterFloat("dryWet", "Dry/Wet", 0, 1, 0.5));
    addParameter(_depthParam = new AudioParameterFloat("depth", "Depth", 0, 1, 0.5));
    addParameter(_rateParam = new AudioParameterFloat("rate", "Rate", 0.1, 20, 10));
    addParameter(_phaseOffsetParam = new AudioParameterFloat("phaseOffset", "Phase Offset", 0, 1, 0));
    addParameter(_feedbackParam = new AudioParameterFloat("feedback", "Feedback", 0, 0.98, 0.5));
    // addParameter(_delayTimeParam = new AudioParameterFloat("delayTime", "Delay Time", 0.01, MAX_DELAY_TIME_IN_SECONDS, 0.5));
    addParameter(_typeParam = new AudioParameterInt("type", "Type", 0, 1, 0));
}

DaalModAudioProcessor::~DaalModAudioProcessor()
{
    // Free up memory for circular buffers if necessary
    if (_circularBufferLeft != nullptr) {
        delete [] _circularBufferLeft;
        _circularBufferLeft = nullptr;
    }
    if (_circularBufferRight != nullptr) {
        delete [] _circularBufferRight;
        _circularBufferRight = nullptr;
    }
}

//==============================================================================
const String DaalModAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DaalModAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DaalModAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DaalModAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DaalModAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DaalModAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int DaalModAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DaalModAudioProcessor::setCurrentProgram (int index)
{
}

const String DaalModAudioProcessor::getProgramName (int index)
{
    return {};
}

void DaalModAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DaalModAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // LFO
    _lfoPhase = 0;
    
    // Calculate circular buffer length based on sample rate
    _circularBufferLength = (int)(sampleRate * MAX_DELAY_TIME_IN_SECONDS);
    
    // Initialize circular buffers based on sample rate and delay time
    // The nullptr setting allows for changing sample rates in the middle of a session
    if (_circularBufferLeft != nullptr) { // Left
        delete [] _circularBufferLeft;
        _circularBufferLeft = nullptr;
    }
    if (_circularBufferLeft == nullptr) {
        _circularBufferLeft = new float[_circularBufferLength];
    }
    zeromem(_circularBufferLeft, _circularBufferLength * sizeof(float));
    
    if (_circularBufferRight != nullptr) { // Right
        delete [] _circularBufferRight;
        _circularBufferRight = nullptr;
    }
    if (_circularBufferRight == nullptr) {
        _circularBufferRight = new float[_circularBufferLength];
    }
    zeromem(_circularBufferLeft, _circularBufferLength * sizeof(float));
    
    // Initialize write head
    _circularBufferWriteHead = 0;
}

void DaalModAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DaalModAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DaalModAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
    //     for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //     {
    //         auto* channelData = buffer.getWritePointer (channel);
    //
    //         // ..do something to the data...
    //     }
    
    // Get write pointers for left and right channels
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    // Debug printing
    // DBG("Dry/Wet: " <<  _dryWetParam->get());
    // DBG("Depth: " <<  _depthParam->get());
    // DBG("Rate: " <<  _rateParam->get());
    // DBG("Phase Offset: " <<  _phaseOffsetParam->get());
    // DBG("Feedback: " <<  _feedbackParam->get());
    // DBG("Type: " <<  _typeParam->get());
    
    // Process audio
    for (int i=0; i<buffer.getNumSamples(); i++) {
        
        // ========
        
        // Set curr LFO out
        float lfoOut = sin(2 * M_PI * _lfoPhase); // Formula for LFO
        
        // Update LFO phase
        _lfoPhase += _rateParam->get() / getSampleRate(); // Increment phase based on rate
        if (_lfoPhase > 1) { // Keep range between 0-1
            _lfoPhase -= 1;
        }
        
        // Apply depth param to LFO out
        lfoOut *= _depthParam->get();
        
        // Map LFO output value to min and max values
        float lfoOutMapped = jmap(lfoOut, -1.0f, 1.0f, LFO_OUT_MIN_IN_SECONDS, LFO_OUT_MAX_IN_SECONDS);
        
        // Update sample rate if need be (use LFO directly since it's already smooth)
        float delayTimeInSamples = getSampleRate() * lfoOutMapped;
        
        // Write sample to circular buffer
        // and also add feedback
        _circularBufferLeft[_circularBufferWriteHead] = leftChannel[i] + _feedbackLeft;
        _circularBufferRight[_circularBufferWriteHead] = rightChannel[i] + _feedbackRight;
        
        // Read from delayed position in buffer
        float delayReadHead = _circularBufferWriteHead - delayTimeInSamples;
        if (delayReadHead < 0) {
            delayReadHead += _circularBufferLength;
        }
        
        // Lerp!
        int delayReadHeadIntX0 = (int) delayReadHead; // x0
        float delayReadHeadRemainderX0 = delayReadHead - delayReadHeadIntX0; // t, i.e. inPhase
        int delayReadHeadIntX1 = delayReadHeadIntX0 + 1; // x1
        if (delayReadHeadIntX1 >= _circularBufferLength) {
            delayReadHeadIntX1 -= _circularBufferLength;
        }
        
        // Get current delay sample for applying feedback
        float delaySampleLeft = lerp(_circularBufferLeft[(int)delayReadHeadIntX0],
                                     _circularBufferLeft[(int)delayReadHeadIntX1],
                                     delayReadHeadRemainderX0);
        float delaySampleRight = lerp(_circularBufferRight[(int)delayReadHeadIntX0],
                                      _circularBufferRight[(int)delayReadHeadIntX1],
                                      delayReadHeadRemainderX0);
        
        // Apply feedback (for next iteration)
        _feedbackLeft = delaySampleLeft * _feedbackParam->get();
        _feedbackRight = delaySampleRight * _feedbackParam->get();
        
        // // Add the samples to the output buffer
        // buffer.addSample(0, i, delaySampleLeft);
        // buffer.addSample(1, i, delaySampleRight);
        
        // Sum the dry and wet (delayed) samples
        buffer.setSample(0, i, (buffer.getSample(0, i) * (1 - _dryWetParam->get())) +
                         (delaySampleLeft * _dryWetParam->get()));
        buffer.setSample(1, i, (buffer.getSample(1, i) * (1 - _dryWetParam->get())) +
                         (delaySampleLeft * _dryWetParam->get()));
        
        
        // Increment write head
        // _circularBufferWriteHead = (_circularBufferWriteHead + 1) % _circularBufferLength;
        _circularBufferWriteHead++;
        if (_circularBufferWriteHead >= _circularBufferLength) {
            _circularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool DaalModAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DaalModAudioProcessor::createEditor()
{
    return new DaalModAudioProcessorEditor (*this);
}

//==============================================================================
void DaalModAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DaalModAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
float DaalModAudioProcessor::lerp(float x0, float x1, float t)
{
    return (1 - t) * x0 + t * x1;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DaalModAudioProcessor();
}

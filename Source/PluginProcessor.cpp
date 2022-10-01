/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#define _USE_MATH_DEFINES
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>
#include "Distort.h"

//==============================================================================
ParadiseAudioProcessor::ParadiseAudioProcessor()
    : parameters(*this, nullptr, juce::Identifier("ParadiseSaves"),
        {
            std::make_unique<juce::AudioParameterFloat>("softClip",            // parameterID
                                                         "softClip",            // parameter name
                                                         0.0f,              // minimum value
                                                         100.0f,              // maximum value
                                                         0.0f),             // default value
            std::make_unique<juce::AudioParameterFloat>("hardClip",            // parameterID
                                                         "hardClip",            // parameter name
                                                         0.0f,              // minimum value
                                                         100.0f,              // maximum value
                                                         0.0f),             // default value
            std::make_unique<juce::AudioParameterFloat>("sinHDistort",            // parameterID
                                                         "sinHDistort",            // parameter name
                                                         0.0f,              // minimum value
                                                         100.0f,              // maximum value
                                                         0.0f),              // default value
            std::make_unique<juce::AudioParameterFloat>("tripleSinDistort",            // parameterID
                                                         "tripleSinDistort",            // parameter name
                                                         0.0f,              // minimum value
                                                         100.0f,              // maximum value
                                                         0.0f),            // default value
            std::make_unique<juce::AudioParameterFloat>("linearDistort",            // parameterID
                                                         "linearDistort",            // parameter name
                                                         0.0f,              // minimum value
                                                         100.0f,              // maximum value
                                                         0.0f)              // default value
        })
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    
#endif
{
    softClipAmount = *parameters.getRawParameterValue("softClip");
    hardClipAmount = *parameters.getRawParameterValue("hardClip");
    sinHAmount = *parameters.getRawParameterValue("sinHDistort");
    tripleSinAmount = *parameters.getRawParameterValue("tripleSinDistort");
    linearAmount = *parameters.getRawParameterValue("linearDistort");
}

ParadiseAudioProcessor::~ParadiseAudioProcessor()
{
}

//==============================================================================
const juce::String ParadiseAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParadiseAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParadiseAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParadiseAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParadiseAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParadiseAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParadiseAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParadiseAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ParadiseAudioProcessor::getProgramName (int index)
{
    return {};
}

void ParadiseAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ParadiseAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ParadiseAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParadiseAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void ParadiseAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
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
    float softAmount = *parameters.getRawParameterValue("softClip");
    float hardAmount = *parameters.getRawParameterValue("hardClip");
    float sAmount = *parameters.getRawParameterValue("sinHDistort");
    float tripAmount = *parameters.getRawParameterValue("tripleSinDistort");
    float linAmount = *parameters.getRawParameterValue("linearDistort");
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            //For each of the types of distortion, check if the amount > 0,
            //then if it is, apply that type of distortion.
            if (softAmount > 0.0) {
                *channelData = distort::softClip(*channelData, softAmount);
            }
            if (hardAmount > 0.0) {
                *channelData = distort::hardClip(*channelData, hardAmount);
            }
            if (sAmount > 0.0) {
                *channelData = distort::sinHDistort(*channelData, sAmount / 5.f);
            }
            if (tripAmount > 0.0) {
                *channelData = distort::tripleSinDistort(*channelData, tripAmount);
            }
            if (linAmount > 0.0) {
                *channelData = distort::tripleSinDistort(*channelData, linAmount);
            }
            channelData++;
        }
        
    }
}

//==============================================================================
bool ParadiseAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ParadiseAudioProcessor::createEditor()
{
    return new ParadiseAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ParadiseAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ParadiseAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParadiseAudioProcessor();
}


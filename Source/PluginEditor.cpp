/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParadiseAudioProcessorEditor::ParadiseAudioProcessorEditor (ParadiseAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (240, 300);
    //These all set the slider's values so that they display and work properly
    // soft clip
    softClip.setSliderStyle(juce::Slider::LinearBarVertical);
    softClip.setRange(0.0, 100.0, .01);
    softClip.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    
    softClip.setValue(audioProcessor.softClipAmount);
    addAndMakeVisible(&softClip);
    softClipAttachment.reset(new SliderAttachment(valueTreeState, "softClip", softClip));
    softClip.addListener(this);

    // hard clip
    hardClip.setSliderStyle(juce::Slider::LinearBarVertical);
    hardClip.setRange(0.0, 100.0, .01);
    hardClip.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    
    hardClip.setValue(audioProcessor.hardClipAmount);
    addAndMakeVisible(&hardClip);
    hardClipAttachment.reset(new SliderAttachment(valueTreeState, "hardClip", hardClip));
    hardClip.addListener(this);

    // sinh
    sinHDistort.setSliderStyle(juce::Slider::LinearBarVertical);
    sinHDistort.setRange(0.0, 100.0, .01);
    sinHDistort.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    
    sinHDistort.setValue(audioProcessor.sinHAmount);
    addAndMakeVisible(&sinHDistort);
    sinHAttachment.reset(new SliderAttachment(valueTreeState, "sinHDistort", sinHDistort));
    sinHDistort.addListener(this);

    // linear
    tripleSinDistort.setSliderStyle(juce::Slider::LinearBarVertical);
    tripleSinDistort.setRange(0.0, 100.0, .01);
    tripleSinDistort.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);

    tripleSinDistort.setValue(audioProcessor.sinHAmount);
    addAndMakeVisible(&tripleSinDistort);
    tripleSinAttachment.reset(new SliderAttachment(valueTreeState, "tripleSinDistort", tripleSinDistort));
    tripleSinDistort.addListener(this);

    // linear
    linearDistort.setSliderStyle(juce::Slider::LinearBarVertical);
    linearDistort.setRange(0.0, 100.0, .01);
    linearDistort.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
   
    linearDistort.setValue(audioProcessor.linearAmount);
    addAndMakeVisible(&linearDistort);
    linearAttachment.reset(new SliderAttachment(valueTreeState, "linearDistort", linearDistort));
    linearDistort.addListener(this);
    //add things to the editor
    

}

ParadiseAudioProcessorEditor::~ParadiseAudioProcessorEditor()
{
}

//==============================================================================
void ParadiseAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Paradise", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void ParadiseAudioProcessorEditor::resized()
{
    // sets the position and size of the sliders with arguments (x, y, width, height)
    softClip.setBounds(40, 30, 20, getHeight() - 60);
    hardClip.setBounds(80, 30, 20, getHeight() - 60);
    sinHDistort.setBounds(120, 30, 20, getHeight() - 60);
    tripleSinDistort.setBounds(160, 30, 20, getHeight() - 60);
    linearDistort.setBounds(200, 30, 20, getHeight() - 60);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void ParadiseAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //Sets the amount in the processor if the slider value changed
    audioProcessor.softClipAmount = softClip.getValue();
    audioProcessor.hardClipAmount = hardClip.getValue();
    audioProcessor.sinHAmount = sinHDistort.getValue();
    audioProcessor.tripleSinAmount = tripleSinDistort.getValue();
    audioProcessor.linearAmount = linearDistort.getValue();
}
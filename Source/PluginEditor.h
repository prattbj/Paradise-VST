/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//==============================================================================
/**
*/
class ParadiseAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                      private juce::Slider::Listener
{
public:
    ParadiseAudioProcessorEditor(ParadiseAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~ParadiseAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParadiseAudioProcessor& audioProcessor;
    void sliderValueChanged(juce::Slider* slider); 
    
    juce::Slider softClip; 
    juce::Slider hardClip;
    juce::Slider sinHDistort;
    juce::Slider tripleSinDistort;
    juce::Slider linearDistort;
    juce::AudioProcessorValueTreeState& valueTreeState;
    std::unique_ptr<SliderAttachment> softClipAttachment;
    std::unique_ptr<SliderAttachment> hardClipAttachment;
    std::unique_ptr<SliderAttachment> sinHAttachment;
    std::unique_ptr<SliderAttachment> tripleSinAttachment;
    std::unique_ptr<SliderAttachment> linearAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParadiseAudioProcessorEditor)
};

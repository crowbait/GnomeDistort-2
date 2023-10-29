/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

enum TreeParameter {

};
enum ChainPositions {

};
struct ChainSettings {

};

//==============================================================================
class GnomeDistort2AudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    GnomeDistort2AudioProcessor();
    ~GnomeDistort2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; };
    const juce::String getName() const override;

    bool acceptsMidi() const override { return false; };
    bool producesMidi() const override { return false; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================

    GnomeDistort2Parameters::ChainSettings chainSettings;
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", GnomeDistort2Parameters::createParameterLayout() };

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GnomeDistort2AudioProcessor);

    //==============================================================================
};

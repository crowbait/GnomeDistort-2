/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GnomeDistort2AudioProcessor::GnomeDistort2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{}

GnomeDistort2AudioProcessor::~GnomeDistort2AudioProcessor() {}

//==============================================================================
const juce::String GnomeDistort2AudioProcessor::getName() const {
    return JucePlugin_Name;
}

double GnomeDistort2AudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int GnomeDistort2AudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}
int GnomeDistort2AudioProcessor::getCurrentProgram() {
    return 0;
}
void GnomeDistort2AudioProcessor::setCurrentProgram(int index) {}
const juce::String GnomeDistort2AudioProcessor::getProgramName(int index) {
    return {};
}
void GnomeDistort2AudioProcessor::changeProgramName(int index, const juce::String& newName) {}
void GnomeDistort2AudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}
#ifndef JucePlugin_PreferredChannelConfigurations
bool GnomeDistort2AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif


void GnomeDistort2AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    spec.sampleRate = sampleRate;
    processorChain.prepare(spec);
}

void GnomeDistort2AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    // for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    //     auto* channelData = buffer.getWritePointer(channel);
    // 
    //     // ..do something to the data...
    // }
    chainSettings.UpdateFromAPVTS(apvts);
    processorChain.updateSettings(chainSettings, getSampleRate());
    processorChain.process(buffer);
}

//==============================================================================
juce::AudioProcessorEditor* GnomeDistort2AudioProcessor::createEditor() {
    // generic interface
    // return new juce::GenericAudioProcessorEditor(*this);
    return new GnomeDistort2AudioProcessorEditor(*this, &apvts, &paramMap);
}
void GnomeDistort2AudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}
void GnomeDistort2AudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    auto state = juce::ValueTree::readFromData(data, sizeInBytes);
    if (state.isValid()) {
        apvts.replaceState(state);
        chainSettings.UpdateFromAPVTS(apvts);
        processorChain.updateSettings(chainSettings, getSampleRate());
    }
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new GnomeDistort2AudioProcessor();
}


//==============================================================================




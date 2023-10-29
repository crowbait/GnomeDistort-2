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

void GnomeDistort2AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}
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

void GnomeDistort2AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);

        // ..do something to the data...
    }
}

//==============================================================================
juce::AudioProcessorEditor* GnomeDistort2AudioProcessor::createEditor() {
    return new GnomeDistort2AudioProcessorEditor(*this);
}
void GnomeDistort2AudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}
void GnomeDistort2AudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new GnomeDistort2AudioProcessor();
}

//==============================================================================


juce::AudioProcessorValueTreeState::ParameterLayout GnomeDistort2AudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("LoCutFreq", "Low-Cut Frequency",                           // Parameter ID, Name
                                                           juce::NormalisableRange<float>(20, 20000, 1, 0.25f),  // Parameter range (20-20k, step-size 1, skew: <1 fills more of the slider with low range
                                                           20));                                                     // default value
    layout.add(std::make_unique<juce::AudioParameterChoice>("LoCutSlope", "Low-Cut Slope",                             // Parameter ID, Name
                                                            FilterSlopeOptions, 1));                                   // Choices StringArray, default index
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandFreqLoMid", "Low/Mid-Band Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 200));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandFreqMidHi", "Mid/High-Band Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 5000));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HiCutFreq", "High-Cut Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 20000));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HiCutSlope", "High-Cut Slope", FilterSlopeOptions, 1));

    // EQ
    juce::NormalisableRange<float> PeakFreqRange = juce::NormalisableRange<float>(0, 1, 1, 0.25f);    // map to freq range of band
    juce::NormalisableRange<float> PeakGainRange = juce::NormalisableRange<float>(-36, 36, 0.25f, 1);
    juce::NormalisableRange<float> PeakQRange = juce::NormalisableRange<float>(0.1f, 10, 0.05f, 1);
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakFreqLo", "Low Band Peak Frequency", PeakFreqRange, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakGainLo", "Low Band Peak Gain", PeakGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakQLo", "Low Band Peak Q", PeakQRange, 1));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakFreqMid", "Mid Band Peak Frequency", PeakFreqRange, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakGainMid", "Mid Band Peak Gain", PeakGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakQMid", "Mid Band Peak Q", PeakQRange, 1));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakFreqHi", "High Band Peak Frequency", PeakFreqRange, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakGainHi", "High Band Peak Gain", PeakGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PeakQHi", "High Band Peak Q", PeakQRange, 1));
    // Dist
    juce::NormalisableRange<float> PreGainRange = juce::NormalisableRange<float>(-8, 32, 0.5f, 1);
    juce::NormalisableRange<float> FdbckAmountRange = juce::NormalisableRange<float>(0, 1, 0.01f, 0.5f);
    juce::NormalisableRange<float> FdbckLengthRange = juce::NormalisableRange<float>(0, 1, 0.01f, 0.5f);
    juce::NormalisableRange<float> WaveShapeAmountRange = juce::NormalisableRange<float>(0, 0.990f, 0.01f, 0.75f);
    juce::NormalisableRange<float> PostGainRange = juce::NormalisableRange<float>(-32, 8, 0.5f, 1);
    layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainLo", "Low Band Pre-Gain", PreGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountLo", "Low Band Feedback Amount", FdbckAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthLo", "Low Band Feedback Length", FdbckLengthRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("WaveShapeAmountLo", "Low Band Distortion Amount", WaveShapeAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("WaveShapeFunctionLo", "Low Band Waveshaper Function", WaveShaperFunctionOptions, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainLo", "Low Band Post-Gain", PostGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainMid", "Mid Band Pre-Gain", PreGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountMid", "Mid Band Feedback Amount", FdbckAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthMid", "Mid Band Feedback Length", FdbckLengthRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("WaveShapeAmountMid", "Mid Band Distortion Amount", WaveShapeAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("WaveShapeFunctionMid", "Mid Band Waveshaper Function", WaveShaperFunctionOptions, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainMid", "Mid Band Post-Gain", PostGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainHi", "High Band Pre-Gain", PreGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountHi", "High Band Feedback Amount", FdbckAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthHi", "High Band Feedback Length", FdbckLengthRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("WaveShapeAmountHi", "High Band Distortion Amount", WaveShapeAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("WaveShapeFunctionHi", "High Band Waveshaper Function", WaveShaperFunctionOptions, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainHi", "High Band Post-Gain", PostGainRange, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("WaveShapeAmountGlobal", "Post Distortion Amount", WaveShapeAmountRange, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("WaveShapeFunctionGlobal", "Post Waveshaper Function", WaveShaperFunctionOptions, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainGlobal", "Global Post-Gain", PostGainRange, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("DryWet", "Dry-Wet Mix", juce::NormalisableRange<float>(0, 1, 0.01f, 1), 1));

    layout.add(std::make_unique<juce::AudioParameterBool>("DisplayON", "DisplayON", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("DisplayHQ", "DisplayHQ", true));
    return layout;
}

/*
  ==============================================================================

    Parameters.h
    Created: 29 Oct 2023 6:04:40pm
    Author:  fabia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ConstOptions.h"

struct GnomeDistort2Parameters {
    enum TreeParameter {
        LoCutFreq, LoCutSlope, BandFreqLoMid, BandFreqMidHi, HiCutFreq, HiCutSlope,
        PeakFreqLo, PeakGainLo, PeakQLo, PeakFreqMid, PeakGainMid, PeakQMid, PeakFreqHi, PeakGainHi, PeakQHi, PreGainLo,
        FdbckAmountLo, FdbckLengthLo, WaveShapeAmountLo, WaveShapeFunctionLo, PostGainLo,
        PreGainMid, FdbckAmountMid, FdbckLengthMid, WaveShapeAmountMid, WaveShapeFunctionMid, PostGainMid,
        PreGainHi, FdbckAmountHi, FdbckLengthHi, WaveShapeAmountHi, WaveShapeFunctionHi, PostGainHi,
        WaveShapeAmountGlobal, WaveShapeFunctionGlobal, PostGainGlobal, DryWet,
        DisplayON, DisplayHQ
    };

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
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
        juce::NormalisableRange<float> PeakFreqRange = juce::NormalisableRange<float>(0, 1, 0.01f, 1);    // map to freq range of band
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
};

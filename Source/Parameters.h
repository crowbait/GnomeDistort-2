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

    struct ChainSettings {
        float LoCutFreq = 0, BandFreqLoMid = 0, BandFreqMidHi = 0, HiCutFreq = 0,
            PeakFreqLo = 0, PeakGainLo = 0, PeakQLo = 0, PeakFreqMid = 0, PeakGainMid, PeakQMid = 0, PeakFreqHi = 0, PeakGainHi = 0, PeakQHi = 0, PreGainLo = 0,
            FdbckAmountLo = 0, FdbckLengthLo = 0, WaveShapeAmountLo = 0, PostGainLo = 0,
            PreGainMid = 0, FdbckAmountMid = 0, FdbckLengthMid = 0, WaveShapeAmountMid = 0, PostGainMid = 0,
            PreGainHi = 0, FdbckAmountHi = 0, FdbckLengthHi = 0, WaveShapeAmountHi = 0, PostGainHi = 0,
            WaveShapeAmountGlobal = 0, PostGainGlobal = 0, DryWet = 0;
        FilterSlope LoCutSlope = FilterSlope::Slope12, HiCutSlope = FilterSlope::Slope12;
        WaveShaperFunction WaveShapeFunctionLo = WaveShaperFunction::HardClip, WaveShapeFunctionMid = WaveShaperFunction::HardClip,
            WaveShapeFunctionHi = WaveShaperFunction::HardClip, WaveShapeFunctionGlobal = WaveShaperFunction::HardClip;

        void UpdateFromAPVTS(juce::AudioProcessorValueTreeState& apvts) {
            LoCutFreq = apvts.getRawParameterValue("LoCutFreq")->load();
            LoCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue("LoCutSlope")->load());
            BandFreqLoMid = apvts.getRawParameterValue("BandFreqLoMid")->load();
            BandFreqMidHi = apvts.getRawParameterValue("BandFreqMidHi")->load();
            HiCutFreq = apvts.getRawParameterValue("HiCutFreq")->load();
            HiCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue("HiCutSlope")->load());
            PeakFreqLo = apvts.getRawParameterValue("PeakFreqLo")->load();
            PeakGainLo = apvts.getRawParameterValue("PeakGainLo")->load();
            PeakQLo = apvts.getRawParameterValue("PeakQLo")->load();
            PeakFreqMid = apvts.getRawParameterValue("PeakFreqMid")->load();
            PeakGainMid = apvts.getRawParameterValue("PeakGainMid")->load();
            PeakQMid = apvts.getRawParameterValue("PeakQMid")->load();
            PeakFreqHi = apvts.getRawParameterValue("PeakFreqHi")->load();
            PeakGainHi = apvts.getRawParameterValue("PeakGainHi")->load();
            PeakQHi = apvts.getRawParameterValue("PeakQHi")->load();
            PreGainLo = apvts.getRawParameterValue("PreGainLo")->load();
            FdbckAmountLo = apvts.getRawParameterValue("FdbckAmountLo")->load();
            FdbckLengthLo = apvts.getRawParameterValue("FdbckLengthLo")->load();
            WaveShapeAmountLo = apvts.getRawParameterValue("WaveShapeAmountLo")->load();
            WaveShapeFunctionLo = static_cast<WaveShaperFunction>(apvts.getRawParameterValue("WaveShapeFunctionLo")->load());
            PostGainLo = apvts.getRawParameterValue("PostGainLo")->load();
            PreGainMid = apvts.getRawParameterValue("PreGainMid")->load();
            FdbckAmountMid = apvts.getRawParameterValue("FdbckAmountMid")->load();
            FdbckLengthMid = apvts.getRawParameterValue("FdbckLengthMid")->load();
            WaveShapeAmountMid = apvts.getRawParameterValue("WaveShapeAmountMid")->load();
            WaveShapeFunctionMid = static_cast<WaveShaperFunction>(apvts.getRawParameterValue("WaveShapeFunctionMid")->load());
            PostGainMid = apvts.getRawParameterValue("PostGainMid")->load();
            PreGainHi = apvts.getRawParameterValue("PreGainHi")->load();
            FdbckAmountHi = apvts.getRawParameterValue("FdbckAmountHi")->load();
            FdbckLengthHi = apvts.getRawParameterValue("FdbckLengthHi")->load();
            WaveShapeAmountHi = apvts.getRawParameterValue("WaveShapeAmountHi")->load();
            WaveShapeFunctionHi = static_cast<WaveShaperFunction>(apvts.getRawParameterValue("WaveShapeFunctionHi")->load());
            PostGainHi = apvts.getRawParameterValue("PostGainHi")->load();
            WaveShapeAmountGlobal = apvts.getRawParameterValue("WaveShapeAmountGlobal")->load();
            WaveShapeFunctionGlobal = static_cast<WaveShaperFunction>(apvts.getRawParameterValue("WaveShapeFunctionGlobal")->load());
            PostGainGlobal = apvts.getRawParameterValue("PostGainGlobal")->load();
            DryWet = apvts.getRawParameterValue("DryWet")->load();
        };
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

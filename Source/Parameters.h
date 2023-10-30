#pragma once
#include <JuceHeader.h>
#include "ConstOptions.h"

struct GnomeDistort2Parameters {
    enum TreeParameter {
        LoCutFreq, LoCutSlope, BandFreqLoMid, BandFreqMidHi, HiCutFreq, HiCutSlope,
        PeakFreqLo, PeakGainLo, PeakQLo, PeakFreqMid, PeakGainMid, PeakQMid, PeakFreqHi, PeakGainHi, PeakQHi, PreGainLo,
        FdbckAmountLo, FdbckLengthLo, WaveshapeAmountLo, WaveshapeFunctionLo, PostGainLo,
        PreGainMid, FdbckAmountMid, FdbckLengthMid, WaveshapeAmountMid, WaveshapeFunctionMid, PostGainMid,
        PreGainHi, FdbckAmountHi, FdbckLengthHi, WaveshapeAmountHi, WaveshapeFunctionHi, PostGainHi,
        WaveshapeAmountGlobal, WaveshapeFunctionGlobal, PostGainGlobal, DryWet,
        DisplayON, DisplayHQ
    };

    struct ChainSettings {
        struct BandChainSettings {
            float PeakFreq = 0, PeakGain = 0, PeakQ = 0, PreGain = 0, FdbckAmount = 0, FdbckLength = 0, WaveshapeAmount = 0, PostGain = 0;
            WaveshaperFunction WaveshapeFunction = WaveshaperFunction::HardClip;
        };

        float LoCutFreq = 0, BandFreqLoMid = 0, BandFreqMidHi = 0, HiCutFreq = 0, WaveshapeAmountGlobal = 0, PostGainGlobal = 0, DryWet = 0;
        FilterSlope LoCutSlope = FilterSlope::Slope12, HiCutSlope = FilterSlope::Slope12;
        WaveshaperFunction WaveshapeFunctionGlobal = WaveshaperFunction::HardClip;
        BandChainSettings LoBandSettings, MidBandSettings, HiBandSettings;
        bool MuteLo = false, SoloLo = false, BypassLo = false, MuteMid = false, SoloMid = false, BypassMid = false, MuteHi = false, SoloHi = false, BypassHi = false;

        void UpdateFromAPVTS(juce::AudioProcessorValueTreeState& apvts) {
            LoCutFreq = apvts.getRawParameterValue("LoCutFreq")->load();
            LoCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue("LoCutSlope")->load());
            BandFreqLoMid = apvts.getRawParameterValue("BandFreqLoMid")->load();
            BandFreqMidHi = apvts.getRawParameterValue("BandFreqMidHi")->load();
            HiCutFreq = apvts.getRawParameterValue("HiCutFreq")->load();
            HiCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue("HiCutSlope")->load());
            LoBandSettings.PeakFreq = apvts.getRawParameterValue("PeakFreqLo")->load();
            LoBandSettings.PeakGain = apvts.getRawParameterValue("PeakGainLo")->load();
            LoBandSettings.PeakQ = apvts.getRawParameterValue("PeakQLo")->load();
            MidBandSettings.PeakFreq = apvts.getRawParameterValue("PeakFreqMid")->load();
            MidBandSettings.PeakGain = apvts.getRawParameterValue("PeakGainMid")->load();
            MidBandSettings.PeakQ = apvts.getRawParameterValue("PeakQMid")->load();
            HiBandSettings.PeakFreq = apvts.getRawParameterValue("PeakFreqHi")->load();
            HiBandSettings.PeakGain = apvts.getRawParameterValue("PeakGainHi")->load();
            HiBandSettings.PeakQ = apvts.getRawParameterValue("PeakQHi")->load();
            LoBandSettings.PreGain = apvts.getRawParameterValue("PreGainLo")->load();
            LoBandSettings.FdbckAmount = apvts.getRawParameterValue("FdbckAmountLo")->load();
            LoBandSettings.FdbckLength = apvts.getRawParameterValue("FdbckLengthLo")->load();
            LoBandSettings.WaveshapeAmount = apvts.getRawParameterValue("WaveshapeAmountLo")->load();
            LoBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue("WaveshapeFunctionLo")->load());
            LoBandSettings.PostGain = apvts.getRawParameterValue("PostGainLo")->load();
            MidBandSettings.PreGain = apvts.getRawParameterValue("PreGainMid")->load();
            MidBandSettings.FdbckAmount = apvts.getRawParameterValue("FdbckAmountMid")->load();
            MidBandSettings.FdbckLength = apvts.getRawParameterValue("FdbckLengthMid")->load();
            MidBandSettings.WaveshapeAmount = apvts.getRawParameterValue("WaveshapeAmountMid")->load();
            MidBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue("WaveshapeFunctionMid")->load());
            MidBandSettings.PostGain = apvts.getRawParameterValue("PostGainMid")->load();
            HiBandSettings.PreGain = apvts.getRawParameterValue("PreGainHi")->load();
            HiBandSettings.FdbckAmount = apvts.getRawParameterValue("FdbckAmountHi")->load();
            HiBandSettings.FdbckLength = apvts.getRawParameterValue("FdbckLengthHi")->load();
            HiBandSettings.WaveshapeAmount = apvts.getRawParameterValue("WaveshapeAmountHi")->load();
            HiBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue("WaveshapeFunctionHi")->load());
            HiBandSettings.PostGain = apvts.getRawParameterValue("PostGainHi")->load();
            WaveshapeAmountGlobal = apvts.getRawParameterValue("WaveshapeAmountGlobal")->load();
            WaveshapeFunctionGlobal = static_cast<WaveshaperFunction>(apvts.getRawParameterValue("WaveshapeFunctionGlobal")->load());
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
        juce::NormalisableRange<float> WaveshapeAmountRange = juce::NormalisableRange<float>(0, 0.990f, 0.01f, 0.75f);
        juce::NormalisableRange<float> PostGainRange = juce::NormalisableRange<float>(-32, 8, 0.5f, 1);
        layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainLo", "Low Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountLo", "Low Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthLo", "Low Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("WaveshapeAmountLo", "Low Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>("WaveshapeFunctionLo", "Low Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainLo", "Low Band Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainMid", "Mid Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountMid", "Mid Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthMid", "Mid Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("WaveshapeAmountMid", "Mid Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>("WaveshapeFunctionMid", "Mid Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainMid", "Mid Band Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PreGainHi", "High Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckAmountHi", "High Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("FdbckLengthHi", "High Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("WaveshapeAmountHi", "High Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>("WaveshapeFunctionHi", "High Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainHi", "High Band Post-Gain", PostGainRange, 0));

        layout.add(std::make_unique<juce::AudioParameterFloat>("WaveshapeAmountGlobal", "Post Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>("WaveshapeFunctionGlobal", "Post Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("PostGainGlobal", "Global Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("DryWet", "Dry-Wet Mix", juce::NormalisableRange<float>(0, 1, 0.01f, 1), 1));

        layout.add(std::make_unique<juce::AudioParameterBool>("MuteLo", "Mute Low Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("SoloLo", "Solo Low Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("BypassLo", "Bypass Low Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("MuteMid", "Mute Mid Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("SoloMid", "Solo Mid Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("BypassMid", "Bypass Mid Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("MuteHi", "Mute High Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("SoloHi", "Solo High Band", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("BypassHi", "Bypass High Band", true));

        layout.add(std::make_unique<juce::AudioParameterBool>("DisplayON", "DisplayON", true));
        layout.add(std::make_unique<juce::AudioParameterBool>("DisplayHQ", "DisplayHQ", true));
        return layout;
    }
};

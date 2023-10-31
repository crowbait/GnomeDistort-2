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
        MuteLo, SoloLo, BypassLo, MuteMid, SoloMid, BypassMid, MuteHi, SoloHi, BypassHi,
        DisplayON, DisplayHQ
    };

    static const std::map<TreeParameter, juce::String> getTreeParameterNames() {
        static const std::map<TreeParameter, juce::String> map = {
            {LoCutFreq, "LoCutFreq"}, {LoCutSlope, "LoCutSlope"}, {BandFreqLoMid, "BandFreqLoMid"}, {BandFreqMidHi, "BandFreqMidHi"}, {HiCutFreq, "HiCutFreq"}, {HiCutSlope, "HiCutSlope"},
            {PeakFreqLo, "PeakFreqLo"}, {PeakGainLo, "PeakGainLo"}, {PeakQLo, "PeakQLo"},
            {PeakFreqMid, "PeakFreqMid"}, {PeakGainMid, "PeakGainMid"}, {PeakQMid, "PeakQMid"},
            {PeakFreqHi, "PeakFreqHi"}, {PeakGainHi, "PeakGainHi"}, {PeakQHi, "PeakQHi"},
            {PreGainLo, "PreGainLo"}, {FdbckAmountLo, "FdbckAmountLo"}, {FdbckLengthLo, "FdbckLengthLo"}, {WaveshapeAmountLo, "WaveshapeAmountLo"}, {WaveshapeFunctionLo, "WaveshapeFunctionLo"}, {PostGainLo, "PostGainLo"},
            {PreGainMid, "PreGainMid"}, {FdbckAmountMid, "FdbckAmountMid"}, {FdbckLengthMid, "FdbckLengthMid"}, {WaveshapeAmountMid, "WaveshapeAmountMid"}, {WaveshapeFunctionMid, "WaveshapeFunctionMid"}, {PostGainMid, "PostGainMid"},
            {PreGainHi, "PreGainHi"}, {FdbckAmountHi, "FdbckAmountHi"}, {FdbckLengthHi, "FdbckLengthHi"}, {WaveshapeAmountHi, "WaveshapeAmountHi"}, {WaveshapeFunctionHi, "WaveshapeFunctionHi"}, {PostGainHi, "PostGainHi"},
            {WaveshapeAmountGlobal, "WaveshapeAmountGlobal"}, {WaveshapeFunctionGlobal, "WaveshapeFunctionGlobal"}, {PostGainGlobal, "PostGainGlobal"}, {DryWet, "DryWet"},
            {MuteLo, "MuteLo"}, {SoloLo, "SoloLo"}, {BypassLo, "BypassLo"}, {MuteMid, "MuteMid"}, {SoloMid, "SoloMid"}, {BypassMid, "BypassMid"}, {MuteHi, "MuteHi"}, {SoloHi, "SoloHi"}, {BypassHi, "BypassHi"},
            {DisplayON, "DisplayON"},
            {DisplayHQ, "DisplayHQ"},
        };
        return map;
    }

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
            std::map<TreeParameter, juce::String> Names = getTreeParameterNames();

            LoCutFreq = apvts.getRawParameterValue(Names.at(TreeParameter::LoCutFreq))->load();
            LoCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue(Names.at(TreeParameter::LoCutSlope))->load());
            BandFreqLoMid = apvts.getRawParameterValue(Names.at(TreeParameter::BandFreqLoMid))->load();
            BandFreqMidHi = apvts.getRawParameterValue(Names.at(TreeParameter::BandFreqMidHi))->load();
            HiCutFreq = apvts.getRawParameterValue(Names.at(TreeParameter::HiCutFreq))->load();
            HiCutSlope = static_cast<FilterSlope>(apvts.getRawParameterValue(Names.at(TreeParameter::HiCutSlope))->load());
            LoBandSettings.PeakFreq = apvts.getRawParameterValue(Names.at(TreeParameter::PeakFreqLo))->load();
            LoBandSettings.PeakGain = apvts.getRawParameterValue(Names.at(TreeParameter::PeakGainLo))->load();
            LoBandSettings.PeakQ = apvts.getRawParameterValue(Names.at(TreeParameter::PeakQLo))->load();
            MidBandSettings.PeakFreq = apvts.getRawParameterValue(Names.at(TreeParameter::PeakFreqMid))->load();
            MidBandSettings.PeakGain = apvts.getRawParameterValue(Names.at(TreeParameter::PeakGainMid))->load();
            MidBandSettings.PeakQ = apvts.getRawParameterValue(Names.at(TreeParameter::PeakQMid))->load();
            HiBandSettings.PeakFreq = apvts.getRawParameterValue(Names.at(TreeParameter::PeakFreqHi))->load();
            HiBandSettings.PeakGain = apvts.getRawParameterValue(Names.at(TreeParameter::PeakGainHi))->load();
            HiBandSettings.PeakQ = apvts.getRawParameterValue(Names.at(TreeParameter::PeakQHi))->load();
            LoBandSettings.PreGain = apvts.getRawParameterValue(Names.at(TreeParameter::PreGainLo))->load();
            LoBandSettings.FdbckAmount = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckAmountLo))->load();
            LoBandSettings.FdbckLength = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckLengthLo))->load();
            LoBandSettings.WaveshapeAmount = apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeAmountLo))->load();
            LoBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeFunctionLo))->load());
            LoBandSettings.PostGain = apvts.getRawParameterValue(Names.at(TreeParameter::PostGainLo))->load();
            MidBandSettings.PreGain = apvts.getRawParameterValue(Names.at(TreeParameter::PreGainMid))->load();
            MidBandSettings.FdbckAmount = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckAmountMid))->load();
            MidBandSettings.FdbckLength = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckLengthMid))->load();
            MidBandSettings.WaveshapeAmount = apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeAmountMid))->load();
            MidBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeFunctionMid))->load());
            MidBandSettings.PostGain = apvts.getRawParameterValue(Names.at(TreeParameter::PostGainMid))->load();
            HiBandSettings.PreGain = apvts.getRawParameterValue(Names.at(TreeParameter::PreGainHi))->load();
            HiBandSettings.FdbckAmount = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckAmountHi))->load();
            HiBandSettings.FdbckLength = apvts.getRawParameterValue(Names.at(TreeParameter::FdbckLengthHi))->load();
            HiBandSettings.WaveshapeAmount = apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeAmountHi))->load();
            HiBandSettings.WaveshapeFunction = static_cast<WaveshaperFunction>(apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeFunctionHi))->load());
            HiBandSettings.PostGain = apvts.getRawParameterValue(Names.at(TreeParameter::PostGainHi))->load();
            WaveshapeAmountGlobal = apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeAmountGlobal))->load();
            WaveshapeFunctionGlobal = static_cast<WaveshaperFunction>(apvts.getRawParameterValue(Names.at(TreeParameter::WaveshapeFunctionGlobal))->load());
            PostGainGlobal = apvts.getRawParameterValue(Names.at(TreeParameter::PostGainGlobal))->load();
            DryWet = apvts.getRawParameterValue(Names.at(TreeParameter::DryWet))->load();
        };
    };

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        std::map<TreeParameter, juce::String> Names = getTreeParameterNames();
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::LoCutFreq), "Low-Cut Frequency",                           // Parameter ID, Name
                                                               juce::NormalisableRange<float>(20, 20000, 1, 0.25f),  // Parameter range (20-20k, step-size 1, skew: <1 fills more of the slider with low range
                                                               20));                                                     // default value
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::LoCutSlope), "Low-Cut Slope",                             // Parameter ID, Name
                                                                FilterSlopeOptions, 1));                                   // Choices StringArray, default index
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::BandFreqLoMid), "Low/Mid-Band Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 200));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::BandFreqMidHi), "Mid/High-Band Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 5000));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::HiCutFreq), "High-Cut Frequency", juce::NormalisableRange<float>(20, 20000, 1, 0.25f), 20000));
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::HiCutSlope), "High-Cut Slope", FilterSlopeOptions, 1));

        // EQ
        juce::NormalisableRange<float> PeakFreqRange = juce::NormalisableRange<float>(0, 1, 0.01f, 1);    // map to freq range of band
        juce::NormalisableRange<float> PeakGainRange = juce::NormalisableRange<float>(-36, 36, 0.25f, 1);
        juce::NormalisableRange<float> PeakQRange = juce::NormalisableRange<float>(0.1f, 10, 0.05f, 1);
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakFreqLo), "Low Band Peak Frequency", PeakFreqRange, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakGainLo), "Low Band Peak Gain", PeakGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakQLo), "Low Band Peak Q", PeakQRange, 1));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakFreqMid), "Mid Band Peak Frequency", PeakFreqRange, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakGainMid), "Mid Band Peak Gain", PeakGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakQMid), "Mid Band Peak Q", PeakQRange, 1));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakFreqHi), "High Band Peak Frequency", PeakFreqRange, 0.5f));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakGainHi), "High Band Peak Gain", PeakGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PeakQHi), "High Band Peak Q", PeakQRange, 1));
        // Dist
        juce::NormalisableRange<float> PreGainRange = juce::NormalisableRange<float>(-8, 32, 0.5f, 1);
        juce::NormalisableRange<float> FdbckAmountRange = juce::NormalisableRange<float>(0, 1, 0.01f, 0.5f);
        juce::NormalisableRange<float> FdbckLengthRange = juce::NormalisableRange<float>(0, 1, 0.01f, 0.5f);
        juce::NormalisableRange<float> WaveshapeAmountRange = juce::NormalisableRange<float>(0, 0.990f, 0.01f, 0.75f);
        juce::NormalisableRange<float> PostGainRange = juce::NormalisableRange<float>(-32, 8, 0.5f, 1);
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PreGainLo), "Low Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckAmountLo), "Low Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckLengthLo), "Low Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::WaveshapeAmountLo), "Low Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::WaveshapeFunctionLo), "Low Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PostGainLo), "Low Band Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PreGainMid), "Mid Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckAmountMid), "Mid Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckLengthMid), "Mid Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::WaveshapeAmountMid), "Mid Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::WaveshapeFunctionMid), "Mid Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PostGainMid), "Mid Band Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PreGainHi), "High Band Pre-Gain", PreGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckAmountHi), "High Band Feedback Amount", FdbckAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::FdbckLengthHi), "High Band Feedback Length", FdbckLengthRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::WaveshapeAmountHi), "High Band Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::WaveshapeFunctionHi), "High Band Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PostGainHi), "High Band Post-Gain", PostGainRange, 0));

        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::WaveshapeAmountGlobal), "Post Distortion Amount", WaveshapeAmountRange, 0));
        layout.add(std::make_unique<juce::AudioParameterChoice>(Names.at(TreeParameter::WaveshapeFunctionGlobal), "Post Waveshaper Function", WaveshaperFunctionOptions, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::PostGainGlobal), "Global Post-Gain", PostGainRange, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>(Names.at(TreeParameter::DryWet), "Dry-Wet Mix", juce::NormalisableRange<float>(0, 1, 0.01f, 1), 1));

        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::MuteLo), "Mute Low Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::SoloLo), "Solo Low Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::BypassLo), "Bypass Low Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::MuteMid), "Mute Mid Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::SoloMid), "Solo Mid Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::BypassMid), "Bypass Mid Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::MuteHi), "Mute High Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::SoloHi), "Solo High Band", false));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::BypassHi), "Bypass High Band", false));

        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::DisplayON), "DisplayON", true));
        layout.add(std::make_unique<juce::AudioParameterBool>(Names.at(TreeParameter::DisplayHQ), "DisplayHQ", true));
        return layout;
    }
};

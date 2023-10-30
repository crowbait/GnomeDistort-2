#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "WaveshaperFunctions.h"

struct GnomeDistort2Processing {
    using Filter = juce::dsp::IIR::Filter<float>;
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
    using Gain = juce::dsp::Gain<float>;
    using Waveshaper = juce::dsp::WaveShaper<float, std::function<float(float)>>;
    using Reverb = juce::dsp::Reverb;

    using Coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(Coefficients& old, const Coefficients& replace) { *old = *replace; };
    static Coefficients generatePeakFilter(const float freq, const float Q, const float gaindB, double sampleRate) {
        return juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate, freq, Q, juce::Decibels::decibelsToGain(gaindB)
        );
    }
    inline auto generateLoCutFilter(const float freq, const FilterSlope slope, double sampleRate) {
        return juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(freq, sampleRate, (slope + 1) * 2);
    }
    inline auto generateLoCutFilter(const float freq, const FilterSlope slope, double sampleRate) {
        return juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(freq, sampleRate, (slope + 1) * 2);
    }
    template<typename ChainType, typename CoefficientsType> static void updateCutFilter(ChainType& filter, const CoefficientsType& cutCoefficients, const FilterSlope& slope) {
        filter.template setBypassed<0>(true);     // bypass all 4 possible filters (one for every possible slope)
        filter.template setBypassed<1>(true);
        filter.template setBypassed<2>(true);
        filter.template setBypassed<3>(true);
        switch (slope) {
            case Slope48: *filter.template get<3>().coefficients = *cutCoefficients[3]; filter.setBypassed<3>(false);
            case Slope36: *filter.template get<2>().coefficients = *cutCoefficients[2]; filter.setBypassed<2>(false);
            case Slope24: *filter.template get<1>().coefficients = *cutCoefficients[1]; filter.setBypassed<0>(false);
            case Slope12: *filter.template get<0>().coefficients = *cutCoefficients[0]; filter.setBypassed<0>(false);
        }
    }

    //==============================================================================

    class DistBand {
    public:
        using BandChain = juce::dsp::ProcessorChain<
            Filter,         // peak
            Gain,           // pre-gain
            Reverb,         // feedback
            Waveshaper,     // dist
            Gain>;          // post-gain
        enum ChainPositions {
            PeakFilter,
            PreGain,
            Reverb,
            Waveshaper,
            PostGain
        };
        BandChain processorChain;
        float LowerFreqBound = 20, UpperFreqBound = 20000;

        void prepare(const juce::dsp::ProcessSpec& spec) { processorChain.prepare(spec); }
        void process(const juce::dsp::ProcessContextReplacing<float>& context) { processorChain.process(context); }

        void updateSettings(const GnomeDistort2Parameters::ChainSettings::BandChainSettings& bandChainSettings, double sampleRate) {
            updateCoefficients(
                processorChain.get<ChainPositions::PeakFilter>().coefficients,
                generatePeakFilter(juce::jmap<float>(bandChainSettings.PeakFreq, LowerFreqBound, UpperFreqBound), bandChainSettings.PeakQ, bandChainSettings.PeakGain, sampleRate)
            );
            processorChain.get<ChainPositions::PreGain>().setGainDecibels(bandChainSettings.PreGain);
            juce::dsp::Reverb::Parameters reverbParams;
            reverbParams.wetLevel = bandChainSettings.FdbckAmount;
            reverbParams.damping = 1.f - bandChainSettings.FdbckAmount;
            reverbParams.roomSize = bandChainSettings.FdbckLength;
            reverbParams.width = bandChainSettings.FdbckLength;
            processorChain.get<ChainPositions::Reverb>().setParameters(reverbParams);
            processorChain.get<ChainPositions::Waveshaper>().functionToUse = GetWaveshaperFunction(bandChainSettings.WaveshapeFunction, bandChainSettings.WaveshapeAmount);
            processorChain.get<ChainPositions::PostGain>().setGainDecibels(bandChainSettings.PostGain);
        }
    };

    //==============================================================================

    class ProcessorChain {
    public:
        GnomeDistort2Processing::DistBand BandLo, BandMid, BandHi;

        void prepare(const juce::dsp::ProcessSpec& spec) {
            // pre-bands prepare
            BandLo.prepare(spec);
            BandMid.prepare(spec);
            BandHi.prepare(spec);
            // post-bands prepare
        }
        void process(const juce::dsp::ProcessContextReplacing<float>& context) {
            // pre-bands processing
            // bands processing
            // post-bands processing
        }
        void updateSettings(const GnomeDistort2Parameters::ChainSettings& chainSettings, double sampleRate) {
            // pre-bands settings

            // bands settings
            BandLo.updateSettings(chainSettings.LoBandSettings, sampleRate);
            BandLo.LowerFreqBound = chainSettings.LoCutFreq;
            BandLo.UpperFreqBound = chainSettings.BandFreqLoMid;
            BandMid.updateSettings(chainSettings.MidBandSettings, sampleRate);
            BandMid.LowerFreqBound = chainSettings.BandFreqLoMid;
            BandMid.UpperFreqBound = chainSettings.BandFreqMidHi;
            BandHi.updateSettings(chainSettings.HiBandSettings, sampleRate);
            BandHi.LowerFreqBound = chainSettings.BandFreqMidHi;
            BandHi.UpperFreqBound = chainSettings.HiCutFreq;
            // post-bands settings

        }
    };

};

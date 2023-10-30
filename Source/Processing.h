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
    static inline auto generateLoCutFilter(const float freq, const FilterSlope slope, double sampleRate) {
        return juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(freq, sampleRate, (slope + 1) * 2);
    }
    static inline auto generateHiCutFilter(const float freq, const FilterSlope slope, double sampleRate) {
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
        BandChain chain;
        float LowerFreqBound = 20, UpperFreqBound = 20000;

        void prepare(const juce::dsp::ProcessSpec& spec) { chain.prepare(spec); }
        void process(const juce::dsp::ProcessContextReplacing<float>& context) { chain.process(context); }

        void updateSettings(const GnomeDistort2Parameters::ChainSettings::BandChainSettings& bandChainSettings, double sampleRate) {
            updateCoefficients(
                chain.get<ChainPositions::PeakFilter>().coefficients,
                generatePeakFilter(juce::jmap<float>(bandChainSettings.PeakFreq, LowerFreqBound, UpperFreqBound), bandChainSettings.PeakQ, bandChainSettings.PeakGain, sampleRate)
            );
            chain.get<ChainPositions::PreGain>().setGainDecibels(bandChainSettings.PreGain);
            juce::dsp::Reverb::Parameters reverbParams;
            reverbParams.wetLevel = bandChainSettings.FdbckAmount;
            reverbParams.damping = 1.f - bandChainSettings.FdbckAmount;
            reverbParams.roomSize = bandChainSettings.FdbckLength;
            reverbParams.width = bandChainSettings.FdbckLength;
            chain.get<ChainPositions::Reverb>().setParameters(reverbParams);
            chain.get<ChainPositions::Waveshaper>().functionToUse = GetWaveshaperFunction(bandChainSettings.WaveshapeFunction, bandChainSettings.WaveshapeAmount);
            chain.get<ChainPositions::PostGain>().setGainDecibels(bandChainSettings.PostGain);
        }
    };

    //==============================================================================

    class GnomeDSP {
    public:
        void prepare(const juce::dsp::ProcessSpec& spec) {
            LP.prepare(spec);
            LP.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
            HP.prepare(spec);
            HP.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
            for (auto& buffer : filterBuffers) buffer.setSize(spec.numChannels, spec.maximumBlockSize);
            // preBandChain.prepare(spec);
            juce::dsp::ProcessSpec monoSpec = spec;
            monoSpec.numChannels = 1;
            BandLoL.prepare(monoSpec);
            BandLoR.prepare(monoSpec);
            BandMidL.prepare(monoSpec);
            BandMidR.prepare(monoSpec);
            BandHiL.prepare(monoSpec);
            BandHiR.prepare(monoSpec);
            postBandChainL.prepare(monoSpec);
            postBandChainR.prepare(monoSpec);
            dryWetMixL.prepare(monoSpec);
            dryWetMixR.prepare(monoSpec);
            dryWetMixL.setMixingRule(juce::dsp::DryWetMixingRule::linear);
            dryWetMixR.setMixingRule(juce::dsp::DryWetMixingRule::linear);
        }
        void process(juce::AudioBuffer<float>& buffer) {
            juce::dsp::AudioBlock<float> block(buffer);
            dryWetMixL.pushDrySamples(block.getSingleChannelBlock(0));
            dryWetMixR.pushDrySamples(block.getSingleChannelBlock(1));

            // pre-bands processing
            // preBandChain.process(juce::dsp::ProcessContextReplacing<float>(block));
            for (auto& fbuffer : filterBuffers) fbuffer = buffer;
            LP.process(juce::dsp::ProcessContextReplacing<float>(juce::dsp::AudioBlock<float>(filterBuffers[0])));
            HP.process(juce::dsp::ProcessContextReplacing<float>(juce::dsp::AudioBlock<float>(filterBuffers[1])));

            int numSamples = buffer.getNumSamples();
            int numChannels = buffer.getNumChannels();
            buffer.clear();
            auto addFilterBand = [numChannels, numSamples](juce::AudioBuffer<float>& inputBuffer, const juce::AudioBuffer<float>& source) {
                for (int i = 0; i < numChannels; ++i) { // ++i is pre-increment: increments BEFORE first loop run
                    inputBuffer.addFrom(i, 0, source, i, 0, numSamples);
                }
            };
            for (int i = 0; i < filterBuffers.size(); i++) addFilterBand(buffer, filterBuffers[i]);

            block = juce::dsp::AudioBlock<float>(buffer);
            juce::dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
            juce::dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
            juce::dsp::ProcessContextReplacing<float> contextL = (blockL);
            juce::dsp::ProcessContextReplacing<float> contextR = (blockR);

            // bands processing

            postBandChainL.process(contextL);
            postBandChainR.process(contextR);
            dryWetMixL.mixWetSamples(blockL);
            dryWetMixR.mixWetSamples(blockR);
        }
        void updateSettings(const GnomeDistort2Parameters::ChainSettings& chainSettings, double sampleRate) {
            // pre-bands settings
            // updateCutFilter(preBandChain.get<0>(), generateLoCutFilter(chainSettings.LoCutFreq, chainSettings.LoCutSlope, sampleRate), chainSettings.LoCutSlope);
            // updateCutFilter(preBandChain.get<1>(), generateLoCutFilter(chainSettings.HiCutFreq, chainSettings.HiCutSlope, sampleRate), chainSettings.HiCutSlope);
            LP.setCutoffFrequency(chainSettings.BandFreqLoMid);
            HP.setCutoffFrequency(chainSettings.BandFreqLoMid);

            // bands settings
            BandLoL.updateSettings(chainSettings.LoBandSettings, sampleRate);
            BandLoR.updateSettings(chainSettings.LoBandSettings, sampleRate);
            BandLoL.LowerFreqBound = chainSettings.LoCutFreq;
            BandLoR.LowerFreqBound = chainSettings.LoCutFreq;
            BandLoL.UpperFreqBound = chainSettings.BandFreqLoMid;
            BandLoR.UpperFreqBound = chainSettings.BandFreqLoMid;
            BandMidL.updateSettings(chainSettings.MidBandSettings, sampleRate);
            BandMidR.updateSettings(chainSettings.MidBandSettings, sampleRate);
            BandMidL.LowerFreqBound = chainSettings.BandFreqLoMid;
            BandMidR.LowerFreqBound = chainSettings.BandFreqLoMid;
            BandMidL.UpperFreqBound = chainSettings.BandFreqMidHi;
            BandMidR.UpperFreqBound = chainSettings.BandFreqMidHi;
            BandHiL.updateSettings(chainSettings.HiBandSettings, sampleRate);
            BandHiR.updateSettings(chainSettings.HiBandSettings, sampleRate);
            BandHiL.LowerFreqBound = chainSettings.BandFreqMidHi;
            BandHiR.LowerFreqBound = chainSettings.BandFreqMidHi;
            BandHiL.UpperFreqBound = chainSettings.HiCutFreq;
            BandHiR.UpperFreqBound = chainSettings.HiCutFreq;

            // post-bands settings
            postBandChainL.get<0>().functionToUse = GetWaveshaperFunction(chainSettings.WaveshapeFunctionGlobal, chainSettings.WaveshapeAmountGlobal);
            postBandChainR.get<0>().functionToUse = GetWaveshaperFunction(chainSettings.WaveshapeFunctionGlobal, chainSettings.WaveshapeAmountGlobal);
            postBandChainL.get<1>().setGainDecibels(chainSettings.PostGainGlobal);
            postBandChainR.get<1>().setGainDecibels(chainSettings.PostGainGlobal);
            dryWetMixL.setWetMixProportion(chainSettings.DryWet);
            dryWetMixR.setWetMixProportion(chainSettings.DryWet);
        }

    private:
        using LRFilter = juce::dsp::LinkwitzRileyFilter<float>;
        LRFilter LP, HP;
        // juce::dsp::ProcessorChain<CutFilter, CutFilter> preBandChain;
        DistBand BandLoL, BandMidL, BandHiL, BandLoR, BandMidR, BandHiR;
        juce::dsp::ProcessorChain<Waveshaper, Gain> postBandChainL, postBandChainR;
        juce::dsp::DryWetMixer<float> dryWetMixL, dryWetMixR;

        std::array<juce::AudioBuffer<float>, 2> filterBuffers;
    };
};

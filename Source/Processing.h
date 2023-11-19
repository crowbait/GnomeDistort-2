#pragma once
#include <JuceHeader.h>
#include "Parameters.h"
#include "WaveshaperFunctions.h"

namespace GnomeDistort2Processing {
    struct Processing {
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
        static inline auto generateLoCutFilter(const float freq, const GnomeDistort2Parameters::Options::FilterSlope slope, double sampleRate) {
            return juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(freq, sampleRate, (slope + 1) * 2);
        }
        static inline auto generateHiCutFilter(const float freq, const GnomeDistort2Parameters::Options::FilterSlope slope, double sampleRate) {
            return juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(freq, sampleRate, (slope + 1) * 2);
        }
        template<typename ChainType, typename CoefficientsType> static void updateCutFilter(ChainType& filter,
                                                                                            const CoefficientsType& cutCoefficients,
                                                                                            const GnomeDistort2Parameters::Options::FilterSlope& slope) {
            filter.template setBypassed<0>(true);     // bypass all 4 possible filters (one for every possible slope)
            filter.template setBypassed<1>(true);
            filter.template setBypassed<2>(true);
            filter.template setBypassed<3>(true);
            switch (slope) {
                case GnomeDistort2Parameters::Options::FilterSlope::Slope48: *filter.template get<3>().coefficients = *cutCoefficients[3]; filter.setBypassed<3>(false);
                case GnomeDistort2Parameters::Options::FilterSlope::Slope36: *filter.template get<2>().coefficients = *cutCoefficients[2]; filter.setBypassed<2>(false);
                case GnomeDistort2Parameters::Options::FilterSlope::Slope24: *filter.template get<1>().coefficients = *cutCoefficients[1]; filter.setBypassed<0>(false);
                case GnomeDistort2Parameters::Options::FilterSlope::Slope12: *filter.template get<0>().coefficients = *cutCoefficients[0]; filter.setBypassed<0>(false);
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

            void updateSettings(const GnomeDistort2Parameters::Parameters::ChainSettings::BandChainSettings& bandChainSettings, double sampleRate);
        };

        //==============================================================================

        class GnomeDSP {
        public:
            void prepare(const juce::dsp::ProcessSpec& spec);
            void process(juce::AudioBuffer<float>& buffer);
            void updateSettings(const GnomeDistort2Parameters::Parameters::ChainSettings& chainSettings, double sampleRate);

        private:
            juce::dsp::ProcessorChain<CutFilter, CutFilter> preBandChainL, preBandChainR;
            using LRFilter = juce::dsp::LinkwitzRileyFilter<float>;
            LRFilter LPLo, APLo, HPMidHi, LPMid, HPHi;  // the AllPass is needed to align delays, see https://youtu.be/Mo0Oco3Vimo?t=9034
            DistBand BandLoL, BandMidL, BandHiL, BandLoR, BandMidR, BandHiR;
            bool isBypassedLo = false, isBypassedMid = false, isBypassedHi = false;
            juce::dsp::ProcessorChain<Waveshaper, Gain> postBandChainL, postBandChainR;
            juce::dsp::DryWetMixer<float> dryWetMixL, dryWetMixR;

            std::array<juce::AudioBuffer<float>, 3> bandBuffers;  // one buffer for each band filter
            unsigned char channelsToAdd = 0b00000111;
        };
    };
}

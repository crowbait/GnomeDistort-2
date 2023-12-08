#pragma once
#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "../../Processing/Parameters.h"
#include "../../Helpers/SingleChannelSampleFifo.h"
#include "../../Helpers/FFTDataGenerator.h"
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct DisplayComponent : juce::Component, juce::AudioProcessorParameter::Listener, juce::Timer {
        DisplayComponent(GnomeDistort2AudioProcessor*,
                         juce::AudioProcessorValueTreeState*,
                         const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*,
                         const GnomeDistort2Theme::Theme*,
                         bool enabled, bool HQ);
        ~DisplayComponent();

        bool isEnabled = true;
        bool isHQ = true;
        juce::Atomic<bool> qualityChanged{ true };

        void timerCallback() override;
        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {} // not implemented

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        GnomeDistort2AudioProcessor* processor;
        GnomeDistort2Processing::Processing::GnomeDSP* DSP;

        juce::Atomic<bool> parametersChanged{ true };   // set true to draw filter curve on launch
        GnomeDistort2Processing::Processing::CutFilter LoCut, HiCut;
        GnomeDistort2Processing::Processing::Filter PeakLo, PeakMid, PeakHi;

        const std::vector<GnomeDistort2Parameters::TreeParameter> paramIndexes = {
            GnomeDistort2Parameters::LoCutFreq,
            GnomeDistort2Parameters::LoCutSlope,
            GnomeDistort2Parameters::HiCutFreq,
            GnomeDistort2Parameters::HiCutSlope,
            GnomeDistort2Parameters::BandFreqLoMid,
            GnomeDistort2Parameters::BandFreqMidHi,
            GnomeDistort2Parameters::PeakFreqLo,
            GnomeDistort2Parameters::PeakGainLo,
            GnomeDistort2Parameters::PeakQLo,
            GnomeDistort2Parameters::PeakFreqMid,
            GnomeDistort2Parameters::PeakGainMid,
            GnomeDistort2Parameters::PeakQMid,
            GnomeDistort2Parameters::PeakFreqHi,
            GnomeDistort2Parameters::PeakGainHi,
            GnomeDistort2Parameters::PeakQHi,
            GnomeDistort2Parameters::BypassLo,
            GnomeDistort2Parameters::BypassMid,
            GnomeDistort2Parameters::BypassHi
        };
        std::vector<juce::RangedAudioParameter*> params;
        std::map<GnomeDistort2Parameters::TreeParameter, float> paramValues;

        const GnomeDistort2Theme::Theme* theme;

        juce::Image background;
        juce::Path filterCurve;
        juce::Path audioCurvePre;
        juce::Path audioCurvePost;

        GnomeDistort2Helpers::SingleChannelSampleFifo<juce::AudioBuffer<float>>* leftPreFifo;
        GnomeDistort2Helpers::SingleChannelSampleFifo<juce::AudioBuffer<float>>* leftPostFifo;
        juce::AudioBuffer<float> preBuffer, postBuffer;
        GnomeDistort2Helpers::FFTDataGenerator<std::vector<float>> preFFTDataGenerator, postFFTDataGenerator;
        GnomeDistort2Helpers::AnalyzerPathGenerator<juce::Path> prePathProducer, postPathProducer;
        void generatePathFromBuffer(GnomeDistort2Helpers::SingleChannelSampleFifo<juce::AudioBuffer<float>>*, GnomeDistort2Helpers::FFTDataGenerator<std::vector<float>>*,
                                    juce::AudioBuffer<float>*, GnomeDistort2Helpers::AnalyzerPathGenerator<juce::Path>*, juce::Path*);

        juce::Rectangle<int> getRenderArea();
        juce::Rectangle<int> getAnalysisArea();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisplayComponent)
    };
};

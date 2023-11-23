#pragma once
#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "../../Parameters.h"

namespace GnomeDistort2Controls {
    struct DisplayComponent : juce::Component, juce::AudioProcessorParameter::Listener, juce::Timer {
        DisplayComponent(GnomeDistort2AudioProcessor*, juce::AudioProcessorValueTreeState*, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*);
        ~DisplayComponent();

        bool isEnabled = true;
        bool isHQ = true;

        void timerCallback() override;
        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {} // not implemented

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        GnomeDistort2AudioProcessor* processor;
        juce::Atomic<bool> parametersChanged{ true };   // set true to draw filter curve on launch

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
            GnomeDistort2Parameters::PeakQHi
        };
        std::vector<juce::RangedAudioParameter*> params;

        juce::Image background;
        juce::Path filterCurve;

        juce::Rectangle<int> getRenderArea();
        juce::Rectangle<int> getAnalysisArea();
    };
};

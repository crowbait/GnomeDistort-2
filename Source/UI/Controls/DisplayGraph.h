#pragma once
#include <JuceHeader.h>
#include "../../Processing/WaveshaperFunctions.h"
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct DisplayGraph : juce::Component, juce::AudioProcessorParameter::Listener {
        DisplayGraph(juce::RangedAudioParameter* func, juce::RangedAudioParameter* amt, const int funcIndex, const int amtIndex, const GnomeDistort2Theme::Theme* theme);
        ~DisplayGraph();

        void paint(juce::Graphics& g) override;
        void resized() override;
        void parameterValueChanged(int parameterIndex, float newValue) override;
        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {} // not implemented

    private:
        int funcParamIndex, amtParamIndex;
        juce::RangedAudioParameter* funcParam;
        juce::RangedAudioParameter* amtParam;
        juce::Image background;
        GnomeDistort2Parameters::Options::WaveshaperFunction func = GnomeDistort2Parameters::Options::WaveshaperFunction::HardClip;
        float amount = 0.f;
        std::function<float(float)> waveshaperFunction = GnomeDistort2Processing::GetWaveshaperFunction(func, amount);

        const GnomeDistort2Theme::Theme* theme;

        juce::Rectangle<int> getRenderArea();
    };
}

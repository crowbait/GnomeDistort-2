#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
    struct DisplayGraph : juce::Component, juce::AudioProcessorParameter::Listener {
        DisplayGraph(juce::RangedAudioParameter* func, juce::RangedAudioParameter* amt, int funcIndex, int amtIndex);
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
        std::function<float(float)> waveshaperFunction;

        juce::Rectangle<int> getRenderArea();
    };
}

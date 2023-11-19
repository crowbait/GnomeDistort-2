#pragma once
#include <JuceHeader.h>

struct DisplayGraph : juce::Component, juce::AudioProcessorParameter::Listener {
    DisplayGraph(juce::AudioParameterChoice*, int, juce::RangedAudioParameter*, int);
    ~DisplayGraph();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {} // not implemented

private:
    int funcParamIndex, amtParamIndex;
    juce::AudioParameterChoice* funcParam;
    juce::RangedAudioParameter* amtParam;
    juce::Image background;
    std::function<float(float)> waveshaperFunction;

    juce::Rectangle<int> getRenderArea();
};

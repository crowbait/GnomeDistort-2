#pragma once
#include <JuceHeader.h>

struct PostBandControls : juce::Component {
    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::gold);
        g.fillAll();
    }
    void resized() override {

    }
};

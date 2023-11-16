#pragma once
#include <JuceHeader.h>

struct PreBandControls : juce::Component {
    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::cyan);
        g.fillAll();
    }
    void resized() override {

    }
};

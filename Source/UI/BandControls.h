#pragma once
#include <JuceHeader.h>

struct BandControls : juce::Component {
    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::orangered);
        g.fillAll();
    }
    void resized() override {

    }
};

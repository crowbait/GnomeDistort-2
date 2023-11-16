#pragma once
#include <JuceHeader.h>

struct DisplayComp : juce::Component {
    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::darkgreen);
        g.fillAll();
    }
    void resized() override {

    }
};

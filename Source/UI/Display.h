#pragma once
#include <JuceHeader.h>
#include "Controls/DisplayComponent.h"
#include "UIConsts.h"

struct Display : juce::Component {
    Display(GnomeDistort2AudioProcessor* processor, juce::AudioProcessorValueTreeState* apvts, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap) :
        displayComp(processor, apvts, paramMap) {

        addAndMakeVisible(&displayComp);
    }

    void paint(juce::Graphics& g) override {
        g.setColour(GnomeDistort2UIConst::COLOR_BG_VERYDARK);
        g.fillAll();
    }
    void resized() override {
        displayComp.setBounds(getLocalBounds());
    }

private:
    GnomeDistort2Controls::DisplayComponent displayComp;
};

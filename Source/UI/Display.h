#pragma once
#include <JuceHeader.h>
#include "../Helpers/Settings.h"
#include "Controls/DisplayComponent.h"
#include "UIConsts.h"

struct Display : juce::Component {
    Display(GnomeDistort2AudioProcessor* processor,
            juce::AudioProcessorValueTreeState* apvts,
            const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
            GnomeDistort2Helpers::Settings* settings) :
        displayComp(processor, apvts, paramMap, settings->displayEnabled, settings->displayHQ) {

        addAndMakeVisible(&displayComp);
    }

    GnomeDistort2Controls::DisplayComponent displayComp;

    void paint(juce::Graphics& g) override {
        g.setColour(GnomeDistort2UIConst::COLOR_BG_VERYDARK);
        g.fillAll();
    }
    void resized() override {
        displayComp.setBounds(getLocalBounds());
    }
};

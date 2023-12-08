#pragma once
#include <JuceHeader.h>
#include "../Helpers/Settings.h"
#include "Controls/DisplayComponent.h"
#include "Theme/Theme.h"

struct Display : juce::Component {
    Display(GnomeDistort2AudioProcessor* processor,
            juce::AudioProcessorValueTreeState* apvts,
            const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
            GnomeDistort2Helpers::Settings* settings,
            const GnomeDistort2Theme::Theme* theme) :
        displayComp(processor, apvts, paramMap, theme, settings->displayEnabled, settings->displayHQ),
        theme(theme) {

        addAndMakeVisible(&displayComp);
    }

    GnomeDistort2Controls::DisplayComponent displayComp;

    const GnomeDistort2Theme::Theme* theme;

    void paint(juce::Graphics& g) override {
        g.setColour(theme->COLOR_BG_VERYDARK);
        g.fillAll();
    }
    void resized() override {
        displayComp.setBounds(getLocalBounds());
    }
    void applyTheme() {
        displayComp.resized();
    }
};

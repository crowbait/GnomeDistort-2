#pragma once
#include <JuceHeader.h>
#include "Controls/DisplayComponent.h"
#include "UIConsts.h"

struct Display : juce::Component {
    Display(GnomeDistort2AudioProcessor* processor) :
        displayComp(processor) {

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

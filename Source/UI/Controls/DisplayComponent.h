#pragma once
#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "../../Parameters.h"

namespace GnomeDistort2Controls {
    struct DisplayComponent : juce::Component, juce::Timer {
        DisplayComponent(GnomeDistort2AudioProcessor*);
        ~DisplayComponent();

        bool isEnabled = true;
        bool isHQ = true;
        void timerCallback() override;

        void paint(juce::Graphics& g) override;
        void resized() override;

    private:
        GnomeDistort2AudioProcessor* processor;

        juce::Image background;
        juce::Rectangle<int> getRenderArea();
        juce::Rectangle<int> getAnalysisArea();
    };
};

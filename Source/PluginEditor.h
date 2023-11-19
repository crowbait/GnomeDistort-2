/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "UI/Display.h"
#include "UI/PreBandControls.h"
#include "UI/BandControls.h"
#include "UI/PostBandControls.h"

class GnomeDistort2AudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor&, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>);
    ~GnomeDistort2AudioProcessorEditor() override;

    enum componentIndex {
        IndDisplay,
        IndPreBandControls,
        IndBandControlsLo,
        IndBandControlsMid,
        IndBandControlsHi,
        IndPostBandControls
    };

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    GnomeDistort2AudioProcessor& audioProcessor;

    DisplayComp Display;
    PreBandControls PreBandControl;
    BandControls BandControlsLo, BandControlsMid, BandControlsHi;
    PostBandControls PostBandControl;

    void checkForUpdates();

    std::vector<juce::Component*> getComponents() {
        return {
            &Display,
            &PreBandControl,
            &BandControlsLo,
            &BandControlsMid,
            &BandControlsHi,
            &PostBandControl
        };
    }

    juce::Image background;
    juce::Image knobOverlay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GnomeDistort2AudioProcessorEditor)
};

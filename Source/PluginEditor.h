#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "Helpers/Settings.h"
#include "UI/Display.h"
#include "UI/PreBandControls.h"
#include "UI/BandControls.h"
#include "UI/PostBandControls.h"
#include "UI/Controls/SliderLabeledValue.h"

class GnomeDistort2AudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor&, juce::AudioProcessorValueTreeState*, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*);
    ~GnomeDistort2AudioProcessorEditor() override;

    GnomeDistort2Helpers::Settings settings;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    Display DisplayArea;
    PreBandControls PreBandControl;
    BandControls BandControlsLo, BandControlsMid, BandControlsHi;
    PostBandControls PostBandControl;
    GnomeDistort2Controls::SliderLabeledValue PostGainSlider, MixSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment AttachPostGainSlider, AttachMixSlider;

    void checkForUpdates();

    std::vector<juce::Component*> getComponents() {
        return {
            &DisplayArea,
            &PreBandControl,
            &BandControlsLo,
            &BandControlsMid,
            &BandControlsHi,
            &PostBandControl,
            &PostGainSlider,
            &MixSlider
        };
    }

    juce::Image background;
    juce::Image knobOverlay;
    juce::Colour primaryColor = GnomeDistort2UIConst::COLOR_PRIMARY, secondaryColor = GnomeDistort2UIConst::COLOR_SECONDARY;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GnomeDistort2AudioProcessorEditor)
};

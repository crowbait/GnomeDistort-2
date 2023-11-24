#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Controls/SliderLabeledValue.h"
#include "Controls/ComboBoxLnF.h"

struct PreBandControls : juce::Component {
    PreBandControls(juce::AudioProcessorValueTreeState*, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*, juce::Image*, juce::Colour*, juce::Colour*);
    ~PreBandControls() {
        LoCutSlopeSelect.setLookAndFeel(nullptr);
        HiCutSlopeSelect.setLookAndFeel(nullptr);
    }

    // child components public for background drawing in Editor
    GnomeDistort2Controls::SliderLabeledValue LoCutSlider, BandLoMidSlider, BandMidHiSlider, HiCutSlider;
    juce::ComboBox LoCutSlopeSelect, HiCutSlopeSelect;

    void paint(juce::Graphics& g) override {};
    void resized() override;

private:
    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS::SliderAttachment AttachLoCutSlider, AttachBandLoMidSlider, AttachBandMidHiSlider, AttachHiCutSlider;
    APVTS::ComboBoxAttachment AttachLoCutSlopeSelect, AttachHiCutSlopeSelect;

    GnomeDistort2Controls::LnFComboBox lnfCombo;

    std::vector<juce::Component*> getComponents() {
        return {
            &LoCutSlider,
            &LoCutSlopeSelect,
            &BandLoMidSlider,
            &BandMidHiSlider,
            &HiCutSlider,
            &HiCutSlopeSelect
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PreBandControls)
};

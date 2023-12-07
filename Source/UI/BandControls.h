#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "Controls/SliderLabeledValue.h"
#include "Controls/DisplayGraph.h"
#include "Controls/ComboBoxLnF.h"
#include "Controls/ToggleButtonLnF.h"
#include "Theme/Theme.h"

enum Band {
    Lo,
    Mid,
    Hi
};
struct BandControls : juce::Component {
    BandControls(const Band, juce::AudioProcessorValueTreeState*, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*, GnomeDistort2Theme::Theme*);
    ~BandControls() {
        WaveshapeFuncSelect.setLookAndFeel(nullptr);
        BtnMute.setLookAndFeel(nullptr);
        BtnSolo.setLookAndFeel(nullptr);
        BtnBypass.setLookAndFeel(nullptr);
    };

    // child components public for background drawing in Editor
    GnomeDistort2Controls::SliderLabeledValue PeakFreqSlider, PeakGainSlider, PeakQSlider, PreGainSlider, SmearAmtSlider, SmearLengthSlider, WaveshapeAmtSlider, PostGainSlider;
    GnomeDistort2Controls::DisplayGraph Display;
    juce::ComboBox WaveshapeFuncSelect;
    juce::TextButton BtnMute{ "M" }, BtnSolo{ "S" }, BtnBypass{ "By" };

    void paint(juce::Graphics& g) override {};
    void resized() override;
    void regenerateLookAndFeel(GnomeDistort2Theme::Theme* theme) {
        lnfCombo.setColors(theme);
        lnfTextToggle.setColors(theme);
    };

private:
    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS::SliderAttachment AttachPeakFreqSlider, AttachPeakGainSlider, AttachPeakQSlider, AttachPreGainSlider, AttachSmearAmtSlider, AttachSmearLengthSlider, AttachWaveshapeAmtSlider, AttachPostGainSlider;
    APVTS::ComboBoxAttachment AttachWaveshapeFuncSelect;
    APVTS::ButtonAttachment AttachBtnSolo, AttachBtnMute, AttachBtnBypass;

    GnomeDistort2Controls::LnFComboBox lnfCombo;
    GnomeDistort2Controls::LnFTextToggleButton lnfTextToggle;

    std::vector<juce::Component*> getComponents() {
        return {
            &PeakFreqSlider,
            &PeakGainSlider,
            &PeakQSlider,
            &PreGainSlider,
            &SmearAmtSlider,
            &SmearLengthSlider,
            &WaveshapeAmtSlider,
            &WaveshapeFuncSelect,
            &Display,
            &PostGainSlider,
            &BtnMute,
            &BtnSolo,
            &BtnBypass
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandControls)
};

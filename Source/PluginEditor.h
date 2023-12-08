#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Processing/Parameters.h"
#include "Helpers/Settings.h"
#include "UI/Display.h"
#include "UI/PreBandControls.h"
#include "UI/BandControls.h"
#include "UI/PostBandControls.h"
#include "UI/Controls/SliderLabeledValue.h"
#include "UI/Controls/TextButton.h"
#include "UI/Controls/TextSwitch.h"
#include "UI/Theme/Theme.h"

class GnomeDistort2AudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor&, juce::AudioProcessorValueTreeState*, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>*);
    ~GnomeDistort2AudioProcessorEditor() override;

    GnomeDistort2Helpers::Settings settings;

    void paint(juce::Graphics&) override;
    void resized() override;

    GnomeDistort2Theme::Theme theme;

private:
    Display DisplayArea;
    PreBandControls PreBandControl;
    BandControls BandControlsLo, BandControlsMid, BandControlsHi;
    PostBandControls PostBandControl;
    GnomeDistort2Controls::SliderLabeledValue PostGainSlider, MixSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment AttachPostGainSlider, AttachMixSlider;
    GnomeDistort2Controls::TextSwitch SwitchDisplayOnButton, SwitchDisplayHQButton;
    GnomeDistort2Controls::TextButton LinkDonateButton, LinkGithubButton;

    void paintBackground();
    void setThemeFromSettings(bool callRedraw);

    std::vector<juce::Component*> getComponents() {
        return {
            &SwitchDisplayOnButton,
            &SwitchDisplayHQButton,
            &LinkDonateButton,
            &LinkGithubButton,
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GnomeDistort2AudioProcessorEditor)
};

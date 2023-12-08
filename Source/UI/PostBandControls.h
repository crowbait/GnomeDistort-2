#pragma once
#include <JuceHeader.h>
#include "Controls/SliderLabeledValue.h"
#include "Controls/ComboBoxLnF.h"
#include "Theme/Theme.h"
#include "Theme/UIConsts.h"

struct PostBandControls : juce::Component {
    PostBandControls(juce::AudioProcessorValueTreeState* apvts,
                     const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
                     const GnomeDistort2Theme::Theme* theme) :
        WaveshapeAmtSlider("DIST", false, &theme->COLOR_PRIMARY, theme),
        lnfCombo(theme),
        AttachWaveshapeAmtSlider(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::WaveshapeAmountGlobal), WaveshapeAmtSlider),
        AttachWaveshapeFuncSelect(*apvts, paramMap->at(GnomeDistort2Parameters::WaveshapeFunctionGlobal), WaveshapeFuncSelect) {

        WaveshapeFuncSelect.addItemList(GnomeDistort2Parameters::Options::WaveshaperFunctionOptions, 1);
        WaveshapeFuncSelect.setSelectedId(apvts->getRawParameterValue(paramMap->at(GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionGlobal))->load() + 1);
        WaveshapeFuncSelect.setLookAndFeel(&lnfCombo);

        for (auto* comp : getComponents()) {
            addAndMakeVisible(comp);
        }
    }
    ~PostBandControls() {
        WaveshapeFuncSelect.setLookAndFeel(nullptr);
    }

    GnomeDistort2Controls::SliderLabeledValue WaveshapeAmtSlider;
    juce::ComboBox WaveshapeFuncSelect;

    void paint(juce::Graphics& g) override {}
    void resized() override {
        auto bounds = getLocalBounds();
        const int padding = GnomeDistort2Theme::COMP_PADDING;

        bounds.removeFromTop((getHeight() - getWidth()) / 2);
        auto distArea = bounds.removeFromTop(getWidth());
        auto funcSelect = distArea.removeFromBottom(GnomeDistort2Theme::SELECT_HEIGHT + padding);
        distArea.expand(-2 * padding, 0);
        distArea.removeFromTop(padding);
        WaveshapeAmtSlider.setBounds(distArea);

        funcSelect.removeFromLeft(padding * 2);
        funcSelect.removeFromRight(padding * 2);
        funcSelect.removeFromTop(padding / 2);
        funcSelect.removeFromBottom(padding / 2);
        WaveshapeFuncSelect.setBounds(funcSelect);
    }
    void applyTheme(const GnomeDistort2Theme::Theme* theme) {
        lnfCombo.setColors(theme);
    };

private:
    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS::SliderAttachment AttachWaveshapeAmtSlider;
    APVTS::ComboBoxAttachment AttachWaveshapeFuncSelect;

    GnomeDistort2Controls::LnFComboBox lnfCombo;

    std::vector<juce::Component*> getComponents() {
        return {
            &WaveshapeAmtSlider,
            &WaveshapeFuncSelect
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PostBandControls)
};

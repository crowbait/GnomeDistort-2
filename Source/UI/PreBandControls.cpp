#include "PreBandControls.h"

PreBandControls::PreBandControls(juce::AudioProcessorValueTreeState* apvts,
                                 const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
                                 juce::Image* knobOverlay,
                                 juce::Colour* primaryColor, juce::Colour* secondaryColor) :
    LoCutSlider("LOCUT", false, knobOverlay, primaryColor, GnomeDistort2Controls::SliderLabeledValue::VALUE_NO_DECIMALS),
    BandLoMidSlider("BAND", false, knobOverlay, primaryColor, GnomeDistort2Controls::SliderLabeledValue::VALUE_NO_DECIMALS),
    BandMidHiSlider("BAND", false, knobOverlay, primaryColor, GnomeDistort2Controls::SliderLabeledValue::VALUE_NO_DECIMALS),
    HiCutSlider("HICUT", false, knobOverlay, primaryColor, GnomeDistort2Controls::SliderLabeledValue::VALUE_NO_DECIMALS),

    AttachLoCutSlider(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::LoCutFreq), LoCutSlider),
    AttachBandLoMidSlider(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::BandFreqLoMid), BandLoMidSlider),
    AttachBandMidHiSlider(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::BandFreqMidHi), BandMidHiSlider),
    AttachHiCutSlider(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::HiCutFreq), HiCutSlider),
    AttachLoCutSlopeSelect(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::LoCutSlope), LoCutSlopeSelect),
    AttachHiCutSlopeSelect(*apvts, paramMap->at(GnomeDistort2Parameters::TreeParameter::HiCutSlope), HiCutSlopeSelect) {

    LoCutSlopeSelect.addItemList(GnomeDistort2Parameters::Options::FilterSlopeOptions, 1);
    LoCutSlopeSelect.setSelectedId(apvts->getRawParameterValue(paramMap->at(GnomeDistort2Parameters::TreeParameter::LoCutSlope))->load() + 1);
    LoCutSlopeSelect.setLookAndFeel(&lnfCombo);
    HiCutSlopeSelect.addItemList(GnomeDistort2Parameters::Options::FilterSlopeOptions, 1);
    HiCutSlopeSelect.setSelectedId(apvts->getRawParameterValue(paramMap->at(GnomeDistort2Parameters::TreeParameter::HiCutSlope))->load() + 1);
    HiCutSlopeSelect.setLookAndFeel(&lnfCombo);

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }
}

void PreBandControls::resized() {
    auto bounds = getLocalBounds();
    const int padding = GnomeDistort2UIConst::COMP_PADDING;

    const int boxWidth = bounds.getHeight();
    auto LoCut = bounds.removeFromLeft(boxWidth);
    auto HiCut = bounds.removeFromRight(boxWidth);

    bounds.removeFromLeft((bounds.getWidth() - (boxWidth * 2)) / 3);
    BandLoMidSlider.setBounds(bounds.removeFromLeft(boxWidth));
    bounds.removeFromLeft((bounds.getWidth() - boxWidth) / 2);
    BandMidHiSlider.setBounds(bounds.removeFromLeft(boxWidth));

    auto LoSlope = LoCut.removeFromBottom(GnomeDistort2UIConst::SELECT_HEIGHT);
    LoCut.removeFromBottom(4);
    LoCutSlider.setBounds(LoCut);
    LoSlope.removeFromLeft(padding);
    LoSlope.removeFromRight(padding);
    LoCutSlopeSelect.setBounds(LoSlope);
    auto HiSlope = HiCut.removeFromBottom(GnomeDistort2UIConst::SELECT_HEIGHT);
    HiCut.removeFromBottom(4);
    HiCutSlider.setBounds(HiCut);
    HiSlope.removeFromLeft(padding);
    HiSlope.removeFromRight(padding);
    HiCutSlopeSelect.setBounds(HiSlope);
}

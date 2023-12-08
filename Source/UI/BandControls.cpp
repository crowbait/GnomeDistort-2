#include "BandControls.h"
#include "Theme/UIConsts.h"

BandControls::BandControls(const Band b,
                           juce::AudioProcessorValueTreeState* apvts,
                           const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
                           const GnomeDistort2Theme::Theme* theme) :
    PeakFreqSlider("FREQ", true, &theme->COLOR_SECONDARY, theme),
    PeakGainSlider("GAIN", true, &theme->COLOR_SECONDARY, theme),
    PeakQSlider("Q", true, &theme->COLOR_SECONDARY, theme),
    PreGainSlider("GAIN", true, &theme->COLOR_PRIMARY, theme),
    SmearAmtSlider("AMT", true, &theme->COLOR_SECONDARY, theme, GnomeDistort2Controls::SliderLabeledValue::NO_VALUE),
    SmearLengthSlider("LNGTH", true, &theme->COLOR_SECONDARY, theme, GnomeDistort2Controls::SliderLabeledValue::NO_VALUE),
    WaveshapeAmtSlider("DIST", false, &theme->COLOR_PRIMARY, theme),
    PostGainSlider("GAIN", true, &theme->COLOR_PRIMARY, theme),

    lnfCombo(theme),
    lnfTextToggle(theme),

    Display(apvts->getParameter(paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                                             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                                             GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi)),
            apvts->getParameter(paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
                                             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
                                             GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi)),
            (b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
             GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi),
            (b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
             GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi), theme),

    AttachPeakFreqSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakFreqLo :
                                              b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakFreqMid :
                                              GnomeDistort2Parameters::TreeParameter::PeakFreqHi), PeakFreqSlider),
    AttachPeakGainSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakGainLo :
                                              b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakGainMid :
                                              GnomeDistort2Parameters::TreeParameter::PeakGainHi), PeakGainSlider),
    AttachPeakQSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakQLo :
                                           b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakQMid :
                                           GnomeDistort2Parameters::TreeParameter::PeakQHi), PeakQSlider),
    AttachPreGainSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PreGainLo :
                                             b == Mid ? GnomeDistort2Parameters::TreeParameter::PreGainMid :
                                             GnomeDistort2Parameters::TreeParameter::PreGainHi), PreGainSlider),
    AttachSmearAmtSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::SmearAmountLo :
                                              b == Mid ? GnomeDistort2Parameters::TreeParameter::SmearAmountMid :
                                              GnomeDistort2Parameters::TreeParameter::SmearAmountHi), SmearAmtSlider),
    AttachSmearLengthSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::SmearLengthLo :
                                                 b == Mid ? GnomeDistort2Parameters::TreeParameter::SmearLengthMid :
                                                 GnomeDistort2Parameters::TreeParameter::SmearLengthHi), SmearLengthSlider),
    AttachWaveshapeAmtSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
                                                  b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
                                                  GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi), WaveshapeAmtSlider),
    AttachPostGainSlider(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PostGainLo :
                                              b == Mid ? GnomeDistort2Parameters::TreeParameter::PostGainMid :
                                              GnomeDistort2Parameters::TreeParameter::PostGainHi), PostGainSlider),

    AttachWaveshapeFuncSelect(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                                                   b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                                                   GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi), WaveshapeFuncSelect),
    AttachBtnMute(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::MuteLo :
                                       b == Mid ? GnomeDistort2Parameters::TreeParameter::MuteMid :
                                       GnomeDistort2Parameters::TreeParameter::MuteHi), BtnMute),
    AttachBtnSolo(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::SoloLo :
                                       b == Mid ? GnomeDistort2Parameters::TreeParameter::SoloMid :
                                       GnomeDistort2Parameters::TreeParameter::SoloHi), BtnSolo),
    AttachBtnBypass(*apvts, paramMap->at(b == Lo ? GnomeDistort2Parameters::TreeParameter::BypassLo :
                                         b == Mid ? GnomeDistort2Parameters::TreeParameter::BypassMid :
                                         GnomeDistort2Parameters::TreeParameter::BypassHi), BtnBypass) {

    WaveshapeFuncSelect.addItemList(GnomeDistort2Parameters::Options::WaveshaperFunctionOptions, 1);
    WaveshapeFuncSelect.setSelectedId(
        apvts->getRawParameterValue(
            paramMap->at(
                b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi)
        )->load() + 1);
    WaveshapeFuncSelect.setLookAndFeel(&lnfCombo);

    BtnMute.setClickingTogglesState(true);
    BtnMute.setLookAndFeel(&lnfTextToggle);
    BtnSolo.setClickingTogglesState(true);
    BtnSolo.setLookAndFeel(&lnfTextToggle);
    BtnBypass.setClickingTogglesState(true);
    BtnBypass.setLookAndFeel(&lnfTextToggle);

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }
}

void BandControls::resized() {
    auto bounds = getLocalBounds();
    const int padding = GnomeDistort2Theme::COMP_PADDING;
    const int thirdsWidth = bounds.getWidth() / 3;
    const int fifthHeight = bounds.getHeight() / 5;

    auto peakArea = bounds.removeFromTop(fifthHeight);
    auto peakGain = peakArea.removeFromLeft(thirdsWidth);
    peakGain.removeFromTop(padding);
    peakGain.removeFromLeft(padding);
    PeakGainSlider.setBounds(peakGain);
    auto peakQ = peakArea.removeFromRight(thirdsWidth);
    peakQ.removeFromTop(padding);
    peakQ.removeFromRight(padding);
    PeakQSlider.setBounds(peakQ);
    PeakFreqSlider.setBounds(peakArea);

    auto preDistArea = bounds.removeFromTop(fifthHeight);
    PreGainSlider.setBounds(preDistArea.removeFromLeft(thirdsWidth));
    auto smearArea = preDistArea.removeFromRight(preDistArea.getWidth() / 1.5f);
    smearArea.expand(padding / 2, padding / 2);
    auto smearLength = juce::Rectangle<int>(smearArea.getX(), smearArea.getY(), smearArea.getWidth(), smearArea.getHeight());
    smearArea.removeFromRight(smearArea.getWidth() / 3 + padding);
    smearArea.removeFromBottom(smearArea.getHeight() / 4);
    SmearAmtSlider.setBounds(smearArea);
    smearLength.removeFromLeft(smearLength.getWidth() / 3 + padding);
    smearLength.removeFromTop(smearLength.getHeight() / 4);
    SmearLengthSlider.setBounds(smearLength);

    auto distArea = bounds.removeFromTop(fifthHeight * 2);
    auto funcSelect = distArea.removeFromBottom(GnomeDistort2Theme::SELECT_HEIGHT + padding);
    auto display = distArea.removeFromRight(distArea.getHeight() - padding * 2);
    WaveshapeAmtSlider.setBounds(distArea);
    display.removeFromRight(padding + (padding / 1.5f));
    display.removeFromLeft(padding - (padding / 1.5f));
    display.removeFromTop(padding * 2.5f);
    display.removeFromBottom(padding * 2.5f);
    Display.setBounds(display);
    funcSelect.removeFromLeft(padding * 2);
    funcSelect.removeFromRight(padding * 2);
    funcSelect.removeFromTop(padding / 2);
    funcSelect.removeFromBottom(padding / 2);
    WaveshapeFuncSelect.setBounds(funcSelect);

    PostGainSlider.setBounds(bounds.removeFromRight(thirdsWidth));

    bounds.removeFromTop(bounds.getHeight() - (padding * 4));   // together with next line, results in button size of padding * 2
    bounds.removeFromBottom(padding * 2);
    bounds.removeFromLeft(padding * 2);

    BtnMute.setBounds(bounds.removeFromLeft(padding * 2));
    bounds.removeFromLeft(padding / 2);
    BtnSolo.setBounds(bounds.removeFromLeft(padding * 2));
    bounds.removeFromLeft(padding / 2);
    BtnBypass.setBounds(bounds.removeFromLeft(padding * 3));
}

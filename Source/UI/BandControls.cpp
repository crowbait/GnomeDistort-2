#include "BandControls.h"

BandControls::BandControls(const Band b,
                           juce::AudioProcessorValueTreeState& apvts,
                           const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>& paramMap,
                           juce::Image& knobOverlay) :
    lnfCombo(),
    PeakFreqSlider("FREQ", true, false, knobOverlay),
    PeakGainSlider("GAIN", true, true, knobOverlay),
    PeakQSlider("Q", true, true, knobOverlay),
    PreGainSlider("GAIN", false, true, knobOverlay),
    SmearAmtSlider("SMEAR AMT", true, true, knobOverlay),
    SmearLengthSlider("SMEAR LNGTH", true, true, knobOverlay),
    WaveshapeAmtSlider("DIST", false, true, knobOverlay),
    PostGainSlider("GAIN", false, true, knobOverlay),

    Display(apvts.getParameter(paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                                           b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                                           GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi)),
            apvts.getParameter(paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
                                           b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
                                           GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi)),
            (b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
             GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi),
            (b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
             b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
             GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi)),

    AttachPeakFreqSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakFreqLo :
                                            b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakFreqMid :
                                            GnomeDistort2Parameters::TreeParameter::PeakFreqHi), PeakFreqSlider),
    AttachPeakGainSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakGainLo :
                                            b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakGainMid :
                                            GnomeDistort2Parameters::TreeParameter::PeakGainHi), PeakGainSlider),
    AttachPeakQSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PeakQLo :
                                         b == Mid ? GnomeDistort2Parameters::TreeParameter::PeakQMid :
                                         GnomeDistort2Parameters::TreeParameter::PeakQHi), PeakQSlider),
    AttachPreGainSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PreGainLo :
                                           b == Mid ? GnomeDistort2Parameters::TreeParameter::PreGainMid :
                                           GnomeDistort2Parameters::TreeParameter::PreGainHi), PreGainSlider),
    AttachSmearAmtSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::SmearAmountLo :
                                            b == Mid ? GnomeDistort2Parameters::TreeParameter::SmearAmountMid :
                                            GnomeDistort2Parameters::TreeParameter::SmearAmountHi), SmearAmtSlider),
    AttachSmearLengthSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::SmearLengthLo :
                                               b == Mid ? GnomeDistort2Parameters::TreeParameter::SmearLengthMid :
                                               GnomeDistort2Parameters::TreeParameter::SmearLengthHi), SmearLengthSlider),
    AttachWaveshapeAmtSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountLo :
                                                b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeAmountMid :
                                                GnomeDistort2Parameters::TreeParameter::WaveshapeAmountHi), WaveshapeAmtSlider),
    AttachPostGainSlider(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::PostGainLo :
                                            b == Mid ? GnomeDistort2Parameters::TreeParameter::PostGainMid :
                                            GnomeDistort2Parameters::TreeParameter::PostGainHi), PostGainSlider),
    AttachWaveshapeFuncSelect(apvts, paramMap.at(b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                                                 b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                                                 GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi), WaveshapeFuncSelect) {

    WaveshapeFuncSelect.setLookAndFeel(&lnfCombo);
    WaveshapeFuncSelect.addItemList(GnomeDistort2Parameters::Options::WaveshaperFunctionOptions, 1);
    WaveshapeFuncSelect.setSelectedId(
        apvts.getRawParameterValue(
            paramMap.at(
                b == Lo ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionLo :
                b == Mid ? GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionMid :
                GnomeDistort2Parameters::TreeParameter::WaveshapeFunctionHi)
        )->load() + 1);

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }
}

void BandControls::resized() {
    auto bounds = getLocalBounds();
    const int padding = 12;
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
    auto smearArea = preDistArea.removeFromRight(preDistArea.getWidth() / 3 * 2);
    auto smearLength = juce::Rectangle<int>(smearArea.getX(), smearArea.getY(), smearArea.getWidth(), smearArea.getHeight());
    smearArea.removeFromRight(smearArea.getWidth() / 3 + (padding / 3));
    smearArea.removeFromBottom(smearArea.getHeight() / 3);
    SmearAmtSlider.setBounds(smearArea);
    smearLength.removeFromLeft(smearLength.getWidth() / 3 + (padding / 3));
    smearLength.removeFromTop(smearLength.getHeight() / 3);
    SmearLengthSlider.setBounds(smearLength);

    auto distArea = bounds.removeFromTop(fifthHeight * 2);
    WaveshapeFuncSelect.setBounds(distArea.removeFromBottom(GnomeDistort2UIConst::SELECT_HEIGHT));
    auto display = distArea.removeFromRight(distArea.getHeight() - padding * 2);
    WaveshapeAmtSlider.setBounds(distArea);
    display.removeFromRight(padding / 2);
    display.removeFromLeft(padding / 2);
    display.removeFromTop(padding * 1.5f);
    display.removeFromBottom(padding * 1.5f);
    Display.setBounds(display);

    PostGainSlider.setBounds(bounds.removeFromRight(thirdsWidth));
}

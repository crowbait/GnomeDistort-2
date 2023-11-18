#include "BandControls.h"

BandControls::BandControls(const Band b,
                           juce::AudioProcessorValueTreeState& apvts,
                           const std::map<TreeParameter, juce::String>& paramMap,
                           const juce::Image& knobOverlay) :
    lnfCombo(),
    PeakFreqSlider(),
    PeakGainSlider(),
    PeakQSlider(),
    PreGainSlider(),
    SmearAmtSlider(),
    SmearLengthSlider(),
    WaveshapeAmtSlider(),
    PostGainSlider(),

    AttachPeakFreqSlider(apvts, paramMap.at(b == Lo ? TreeParameter::PeakFreqLo : b == Mid ? TreeParameter::PeakFreqMid : TreeParameter::PeakFreqHi), PeakFreqSlider),
    AttachPeakGainSlider(apvts, paramMap.at(b == Lo ? TreeParameter::PeakGainLo : b == Mid ? TreeParameter::PeakGainMid : TreeParameter::PeakGainHi), PeakGainSlider),
    AttachPeakQSlider(apvts, paramMap.at(b == Lo ? TreeParameter::PeakQLo : b == Mid ? TreeParameter::PeakQMid : TreeParameter::PeakQHi), PeakQSlider),
    AttachPreGainSlider(apvts, paramMap.at(b == Lo ? TreeParameter::PreGainLo : b == Mid ? TreeParameter::PreGainMid : TreeParameter::PreGainHi), PreGainSlider),
    AttachSmearAmtSlider(apvts, paramMap.at(b == Lo ? TreeParameter::SmearAmountLo : b == Mid ? TreeParameter::SmearAmountMid : TreeParameter::SmearAmountHi), SmearAmtSlider),
    AttachSmearLengthSlider(apvts, paramMap.at(b == Lo ? TreeParameter::SmearLengthLo : b == Mid ? TreeParameter::SmearLengthMid : TreeParameter::SmearLengthHi), SmearLengthSlider),
    AttachWaveshapeAmtSlider(apvts, paramMap.at(b == Lo ? TreeParameter::WaveshapeAmountLo : b == Mid ? TreeParameter::WaveshapeAmountMid : TreeParameter::WaveshapeAmountHi), WaveshapeAmtSlider),
    AttachPostGainSlider(apvts, paramMap.at(b == Lo ? TreeParameter::PostGainLo : b == Mid ? TreeParameter::PostGainMid : TreeParameter::PostGainHi), PostGainSlider),
    AttachWaveshapeFuncSelect(apvts, paramMap.at(b == Lo ? TreeParameter::WaveshapeFunctionLo : b == Mid ? TreeParameter::WaveshapeFunctionMid : TreeParameter::WaveshapeFunctionHi), WaveshapeFuncSelect) {

    WaveshapeFuncSelect.setLookAndFeel(&lnfCombo);
    WaveshapeFuncSelect.addItemList(WaveshaperFunctionOptions, 1);
    WaveshapeFuncSelect.setSelectedId(
        apvts.getRawParameterValue(
            paramMap.at(
                b == Lo ? TreeParameter::WaveshapeFunctionLo :
                b == Mid ? TreeParameter::WaveshapeFunctionMid :
                TreeParameter::WaveshapeFunctionHi)
        )->load() + 1);

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }
}

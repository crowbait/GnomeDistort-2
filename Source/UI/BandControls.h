#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Parameters.h"
#include "Controls/SliderLabeledValue.h"
#include "Controls/DisplayGraph.h"
#include "Controls/ComboBox.h"

enum Band {
    Lo,
    Mid,
    Hi
};
struct BandControls : juce::Component {
    BandControls(const Band, juce::AudioProcessorValueTreeState&, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>&, juce::Image&);
    ~BandControls() { WaveshapeFuncSelect.setLookAndFeel(nullptr); };

    enum componentIndex {
        IndPeakFreq,
        IndPeakGain,
        IndPeakQ,
        IndPreGain,
        IndSmearAmt,
        IndSmearLength,
        IndWaveshapeAmt,
        IndWaveshapeFunc,
        IndWaveshapeDisplay,
        IndPostGain
    };

    void paint(juce::Graphics& g) override {};
    void resized() override;

private:
    GnomeDistort2Controls::SliderLabeledValue PeakFreqSlider, PeakGainSlider, PeakQSlider, PreGainSlider, SmearAmtSlider, SmearLengthSlider, WaveshapeAmtSlider, PostGainSlider;
    GnomeDistort2Controls::DisplayGraph Display;
    juce::ComboBox WaveshapeFuncSelect;
    using APVTS = juce::AudioProcessorValueTreeState;
    APVTS::SliderAttachment AttachPeakFreqSlider, AttachPeakGainSlider, AttachPeakQSlider, AttachPreGainSlider, AttachSmearAmtSlider, AttachSmearLengthSlider, AttachWaveshapeAmtSlider, AttachPostGainSlider;
    APVTS::ComboBoxAttachment AttachWaveshapeFuncSelect;

    GnomeDistort2Controls::LnFComboBox lnfCombo;

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
            &PostGainSlider
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandControls)
};

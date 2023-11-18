#pragma once
#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../Parameters.h"
#include "Controls/SliderLabeledValue.h"
#include "Controls/ComboBox.h"

enum Band {
    Lo,
    Mid,
    Hi
};
struct BandControls : juce::Component {
    BandControls(const Band, juce::AudioProcessorValueTreeState&, const std::map<TreeParameter, juce::String>&, const juce::Image&);
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
        IndPostGain
    };

    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::orangered);
        g.fillAll();
    }
    void resized() override {

    }

private:
    GnomeDistort2Controls::SliderLabeledValue PeakFreqSlider, PeakGainSlider, PeakQSlider, PreGainSlider, SmearAmtSlider, SmearLengthSlider, WaveshapeAmtSlider, PostGainSlider;
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
            &SmearAmtSlider,
            &SmearLengthSlider,
            &WaveshapeAmtSlider,
            &WaveshapeFuncSelect,
            &PostGainSlider
        };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandControls)
};

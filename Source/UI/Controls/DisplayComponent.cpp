#include "DisplayComponent.h"
#include "../../Processing.h"
#include "../UIConsts.h"

GnomeDistort2Controls::DisplayComponent::DisplayComponent(GnomeDistort2AudioProcessor* processorPointer,
                                                          juce::AudioProcessorValueTreeState* apvts,
                                                          const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap) :
    processor(processorPointer) {

    params.resize(paramIndexes.size());
    for (int i = 0; i < params.size(); i++) {
        params[i] = apvts->getParameter(paramMap->at(paramIndexes[i]));
        params[i]->addListener(this);
    }
    startTimerHz(30);
}
GnomeDistort2Controls::DisplayComponent::~DisplayComponent() {
    for (auto param : params) {
        param->removeListener(this);
    }
}

void GnomeDistort2Controls::DisplayComponent::resized() {

}

void GnomeDistort2Controls::DisplayComponent::paint(juce::Graphics& g) {
    using namespace juce;

    auto displayArea = getLocalBounds();
    auto renderArea = getRenderArea();
    auto analysisArea = getAnalysisArea();
    const int analysisWidth = analysisArea.getWidth();
    const double outputMin = analysisArea.getBottom();
    const double outputMax = analysisArea.getY();

    g.setColour(GnomeDistort2UIConst::COLOR_BG_VERYDARK);
    g.fillRect(displayArea.toFloat());
    g.drawImage(background, displayArea.toFloat());

    g.setColour(Colours::white);
    g.strokePath(filterCurve, PathStrokeType(2));
}

void GnomeDistort2Controls::DisplayComponent::parameterValueChanged(int parameterIndex, float newValue) {
    parametersChanged.set(true);
}

void GnomeDistort2Controls::DisplayComponent::timerCallback() {
    using namespace juce;

    // if filter params have changed, regenerate new path
    if (parametersChanged.compareAndSetBool(false, true)) {
        GnomeDistort2Processing::Processing::GnomeDSP* DSP = &processor->processorChain;
        auto analysisArea = getAnalysisArea();
        const int analysisWidth = analysisArea.getWidth();
        const double outputMin = analysisArea.getBottom();
        const double outputMax = analysisArea.getY();

        // filter curve
        std::vector<double> magnitudes;
        magnitudes.resize(analysisWidth);
        const double sampleRate = processor->getSampleRate();
        GnomeDistort2Processing::Processing::CutFilter* loCut = &DSP->preBandChainL.get<0>();
        GnomeDistort2Processing::Processing::CutFilter* hiCut = &DSP->preBandChainL.get<1>();

        for (int i = 0; i < analysisWidth; i++) {
            double freq = mapToLog10((double)i / (double)analysisWidth, 20.0, 20000.0); // X axis
            double magnitude = 1.0;                                                     // Y axis

            if (!loCut->isBypassed<0>()) magnitude *= loCut->get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!loCut->isBypassed<1>()) magnitude *= loCut->get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!loCut->isBypassed<2>()) magnitude *= loCut->get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!loCut->isBypassed<3>()) magnitude *= loCut->get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!hiCut->isBypassed<0>()) magnitude *= hiCut->get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!hiCut->isBypassed<1>()) magnitude *= hiCut->get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!hiCut->isBypassed<2>()) magnitude *= hiCut->get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!hiCut->isBypassed<3>()) magnitude *= hiCut->get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);

            if (!DSP->isBypassedLo) magnitude *= DSP->BandLoL.chain.get<GnomeDistort2Processing::Processing::DistBand::PosPeakFilter>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!DSP->isBypassedMid) magnitude *= DSP->BandMidL.chain.get<GnomeDistort2Processing::Processing::DistBand::PosPeakFilter>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!DSP->isBypassedHi) magnitude *= DSP->BandHiL.chain.get<GnomeDistort2Processing::Processing::DistBand::PosPeakFilter>().coefficients->getMagnitudeForFrequency(freq, sampleRate);

            magnitudes[i] = Decibels::gainToDecibels(magnitude);
        }

        filterCurve.clear();
        auto map = [outputMin, outputMax](double input) { return jmap(input, -36.0, 36.0, outputMin, outputMax); };
        filterCurve.startNewSubPath(analysisArea.getX(), map(magnitudes.front()));
        for (int i = 1; i < magnitudes.size(); i++) {   // set path for every pixel
            filterCurve.lineTo(analysisArea.getX() + i, map(magnitudes[i]));
        }
    }

    juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
    if (mml.lockWasGained()) {
        repaint();
    } else DBG("No Lock");
}

juce::Rectangle<int> GnomeDistort2Controls::DisplayComponent::getRenderArea() {
    auto bounds = getLocalBounds();
    bounds.removeFromLeft(24);
    bounds.removeFromRight(24);
    bounds.removeFromTop(12);
    bounds.removeFromBottom(12);
    return bounds;
}
juce::Rectangle<int> GnomeDistort2Controls::DisplayComponent::getAnalysisArea() {
    auto bounds = getRenderArea();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    bounds.removeFromRight(4);
    return bounds;
}

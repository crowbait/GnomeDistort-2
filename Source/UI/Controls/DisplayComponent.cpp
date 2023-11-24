#include "DisplayComponent.h"
#include "../UIConsts.h"

GnomeDistort2Controls::DisplayComponent::DisplayComponent(GnomeDistort2AudioProcessor* processorPointer,
                                                          juce::AudioProcessorValueTreeState* apvts,
                                                          const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
                                                          bool enabled, bool HQ) :
    processor(processorPointer) {

    DSP = &processor->processorChain;
    leftPreFifo = &DSP->leftPreProcessingFifo;
    leftPostFifo = &DSP->leftPostProcessingFifo;

    isEnabled = enabled;
    isHQ = HQ;

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

void GnomeDistort2Controls::DisplayComponent::parameterValueChanged(int parameterIndex, float newValue) {
    parametersChanged.set(true);
}

void GnomeDistort2Controls::DisplayComponent::generatePathFromBuffer(GnomeDistort2Helpers::SingleChannelSampleFifo<juce::AudioBuffer<float>>* fifo,
                                                                     GnomeDistort2Helpers::FFTDataGenerator<std::vector<float>>* FFTGen,
                                                                     juce::AudioBuffer<float>* buffer,
                                                                     GnomeDistort2Helpers::AnalyzerPathGenerator<juce::Path>* pathProducer,
                                                                     juce::Path* path) {
    const float negInfinity = -48.f;
    juce::AudioBuffer<float> tempBuffer;
    while (fifo->getNumCompletedBuffersAvailable() > 0) {
        if (fifo->getAudioBuffer(tempBuffer)) {
            int size = tempBuffer.getNumSamples();
            juce::FloatVectorOperations::copy(buffer->getWritePointer(0, 0), buffer->getReadPointer(0, size), buffer->getNumSamples() - size);
            juce::FloatVectorOperations::copy(buffer->getWritePointer(0, buffer->getNumSamples() - size), tempBuffer.getReadPointer(0, 0), size);
            FFTGen->produceFFTData(*buffer, negInfinity);
        }
    }
    const auto fftBounds = getAnalysisArea().toFloat();
    const int fftSize = FFTGen->getFFTSize();
    const float binWidth = processor->getSampleRate() / (double)fftSize;
    while (FFTGen->getNumAvailableFFTDataBlocks() > 0) {    // generate paths from FFT data
        std::vector<float> fftData;
        if (FFTGen->getFFTData(fftData)) {
            pathProducer->generatePath(fftData, fftBounds, fftSize, binWidth, negInfinity, false);
        }
    }
    while (pathProducer->getNumPathsAvailable() > 0) {    // pull paths as long as there are any, draw the most recent one
        pathProducer->getPath(*path);
    }
}

void GnomeDistort2Controls::DisplayComponent::timerCallback() {
    using namespace juce;

    // if filter params have changed, regenerate new path
    if (parametersChanged.compareAndSetBool(false, true)) {
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

        if (!isEnabled) {   // repaint on parameter change even if display not enabled
            juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
            if (mml.lockWasGained()) {
                repaint();
            } else DBG("No Lock");
        }
    }

    if (isEnabled) {
        if (qualityChanged.compareAndSetBool(false, true)) {
            preFFTDataGenerator.changeOrder(isHQ ? GnomeDistort2Helpers::FFTOrder::order8192 : GnomeDistort2Helpers::FFTOrder::order2048);
            postFFTDataGenerator.changeOrder(isHQ ? GnomeDistort2Helpers::FFTOrder::order8192 : GnomeDistort2Helpers::FFTOrder::order2048);
            preBuffer.setSize(1, preFFTDataGenerator.getFFTSize());
            postBuffer.setSize(1, postFFTDataGenerator.getFFTSize());

            stopTimer();
            startTimerHz(isHQ ? 30 : 15);
        }

        generatePathFromBuffer(leftPreFifo, &preFFTDataGenerator, &preBuffer, &prePathProducer, &audioCurvePre);
        generatePathFromBuffer(leftPostFifo, &postFFTDataGenerator, &postBuffer, &postPathProducer, &audioCurvePost);

        juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
        if (mml.lockWasGained()) {
            repaint();
        } else DBG("No Lock");
    }
}

juce::Rectangle<int> GnomeDistort2Controls::DisplayComponent::getRenderArea() {
    auto bounds = getLocalBounds();
    bounds.removeFromLeft(24);
    bounds.removeFromRight(8);
    bounds.removeFromTop(12);
    bounds.removeFromBottom(4);
    return bounds;
}
juce::Rectangle<int> GnomeDistort2Controls::DisplayComponent::getAnalysisArea() {
    auto bounds = getRenderArea();
    bounds.removeFromTop(4);
    bounds.removeFromBottom(4);
    bounds.removeFromRight(4);
    return bounds;
}

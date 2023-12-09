#include "DisplayComponent.h"

GnomeDistort2Controls::DisplayComponent::DisplayComponent(GnomeDistort2AudioProcessor* processorPointer,
                                                          juce::AudioProcessorValueTreeState* apvts,
                                                          const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* paramMap,
                                                          const GnomeDistort2Theme::Theme* theme,
                                                          bool enabled, bool HQ) :
    processor(processorPointer), theme(theme) {

    DSP = &processor->processorChain;
    leftPreFifo = &DSP->leftPreProcessingFifo;
    leftPostFifo = &DSP->leftPostProcessingFifo;
    isEnabled = enabled;
    isHQ = HQ;

    setOpaque(true);

    params.resize(paramIndexes.size());
    for (int i = 0; i < params.size(); i++) {
        params[i] = apvts->getParameter(paramMap->at(paramIndexes[i]));
        params[i]->addListener(this);
        paramValues[paramIndexes[i]] = apvts->getRawParameterValue(paramMap->at(paramIndexes[i]))->load();
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

    using namespace GnomeDistort2Processing;
    using namespace GnomeDistort2Parameters;

    // map 0-1 newValue to actual parameter ranges
    switch (parameterIndex) {
        case LoCutFreq:
        case HiCutFreq:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = CutoffRange.convertFrom0to1(newValue);
            break;
        case LoCutSlope:
        case HiCutSlope:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = juce::jmap(newValue, (float)Options::Slope12, (float)Options::Slope48);
            break;
        case BandFreqLoMid:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = LoMidBandRange.convertFrom0to1(newValue);
            break;
        case BandFreqMidHi:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = MidHiBandRange.convertFrom0to1(newValue);
            break;
        case PeakQLo:
        case PeakQMid:
        case PeakQHi:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = PeakQRange.convertFrom0to1(newValue);
            break;
        case PeakGainLo:
        case PeakGainMid:
        case PeakGainHi:
            paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = PeakGainRange.convertFrom0to1(newValue);
            break;
        default: paramValues[static_cast<GnomeDistort2Parameters::TreeParameter>(parameterIndex)] = newValue;
    }

    // update local filters (getting them from DSP is a race condition)
    const double sampleRate = processor->getSampleRate();
    Options::FilterSlope LoSlope = static_cast<Options::FilterSlope>(paramValues[LoCutSlope]);
    Options::FilterSlope HiSlope = static_cast<Options::FilterSlope>(paramValues[HiCutSlope]);
    auto preBandsLoCut = Processing::generateLoCutFilter(paramValues[LoCutFreq], LoSlope, sampleRate);
    auto preBandsHiCut = Processing::generateHiCutFilter(paramValues[HiCutFreq], HiSlope, sampleRate);
    Processing::updateCutFilter<Processing::CutFilter, juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>>>(LoCut, preBandsLoCut, LoSlope);
    Processing::updateCutFilter<Processing::CutFilter, juce::ReferenceCountedArray<juce::dsp::IIR::Coefficients<float>>>(HiCut, preBandsHiCut, HiSlope);
    Processing::updateCoefficients(PeakLo.coefficients, Processing::generatePeakFilter(juce::jmap<float>(paramValues[PeakFreqLo], 20, paramValues[BandFreqLoMid]),
                                                                                       paramValues[PeakQLo], paramValues[PeakGainLo], sampleRate));
    Processing::updateCoefficients(PeakMid.coefficients, Processing::generatePeakFilter(juce::jmap<float>(paramValues[PeakFreqMid], paramValues[BandFreqLoMid], paramValues[BandFreqMidHi]),
                                                                                        paramValues[PeakQMid], paramValues[PeakGainMid], sampleRate));
    Processing::updateCoefficients(PeakHi.coefficients, Processing::generatePeakFilter(juce::jmap<float>(paramValues[PeakFreqHi], paramValues[BandFreqMidHi], 20000),
                                                                                       paramValues[PeakQHi], paramValues[PeakGainHi], sampleRate));

    if (!isEnabled) {
        juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
        if (mml.lockWasGained()) {
            repaint();
        } else DBG("No Lock");
    }
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

juce::Rectangle<int> GnomeDistort2Controls::DisplayComponent::getRenderArea() const {
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

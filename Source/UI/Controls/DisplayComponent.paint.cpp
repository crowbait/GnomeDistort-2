#include "DisplayComponent.h"
#include "../Theme/UIConsts.h"

void GnomeDistort2Controls::DisplayComponent::paint(juce::Graphics& g) {
    using namespace juce;
    using namespace GnomeDistort2Theme;

    auto displayArea = getLocalBounds();
    auto analysisArea = getAnalysisArea();
    const int analysisX = analysisArea.getX();
    const int analysisWidth = analysisArea.getWidth();
    const int analysisHeight = analysisArea.getHeight();
    const double outputMin = analysisArea.getBottom();
    const double outputMax = analysisArea.getY();
    const int BandLoMidX = mapFromLog10(DSP->BandMidL.LowerFreqBound, 20.f, 20000.f) * analysisWidth + analysisX;
    const int BandMidHiX = mapFromLog10(DSP->BandMidL.UpperFreqBound, 20.f, 20000.f) * analysisWidth + analysisX;

    // background
    g.drawImage(background, displayArea.toFloat());

    // band amplitudes if not disabled
    if (isEnabled) {
        const float BandLoHeight = jlimit(0.f, (float)analysisHeight, DSP->AmplitudeLo * 0.9f * analysisHeight);
        const float BandMidHeight = jlimit(0.f, (float)analysisHeight, DSP->AmplitudeMid * 0.9f * analysisHeight);
        const float BandHiHeight = jlimit(0.f, (float)analysisHeight, DSP->AmplitudeHi * 0.9f * analysisHeight);
        Rectangle<int> LoBand(analysisX, analysisHeight - BandLoHeight + outputMax, BandLoMidX - analysisX, BandLoHeight);
        Rectangle<int> MidBand(BandLoMidX, analysisHeight - BandMidHeight + outputMax, BandMidHiX - BandLoMidX, BandMidHeight);
        Rectangle<int> HiBand(BandMidHiX, analysisHeight - BandHiHeight + outputMax, analysisArea.getRight() - BandMidHiX, BandHiHeight);
        const float LoDist = processor->chainSettings.LoBandSettings.WaveshapeAmount;
        const float MidDist = processor->chainSettings.MidBandSettings.WaveshapeAmount;
        const float HiDist = processor->chainSettings.HiBandSettings.WaveshapeAmount;

        g.setColour(Colour((uint8)std::floor(jmap(LoDist, (float)theme->COLOR_GRAPH_SECONDARY.getRed(), (float)theme->COLOR_GRAPH_PRIMARY.getRed())),
                           (uint8)std::floor(jmap(LoDist, (float)theme->COLOR_GRAPH_SECONDARY.getGreen(), (float)theme->COLOR_GRAPH_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(LoDist, (float)theme->COLOR_GRAPH_SECONDARY.getBlue(), (float)theme->COLOR_GRAPH_PRIMARY.getBlue())), 0.6f));
        g.fillRect(LoBand.toFloat());
        g.setColour(Colour((uint8)std::floor(jmap(MidDist, (float)theme->COLOR_GRAPH_SECONDARY.getRed(), (float)theme->COLOR_GRAPH_PRIMARY.getRed())),
                           (uint8)std::floor(jmap(MidDist, (float)theme->COLOR_GRAPH_SECONDARY.getGreen(), (float)theme->COLOR_GRAPH_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(MidDist, (float)theme->COLOR_GRAPH_SECONDARY.getBlue(), (float)theme->COLOR_GRAPH_PRIMARY.getBlue())), 0.6f));
        g.fillRect(MidBand.toFloat());
        g.setColour(Colour((uint8)std::floor(jmap(HiDist, (float)theme->COLOR_GRAPH_SECONDARY.getRed(), (float)theme->COLOR_GRAPH_PRIMARY.getRed())),
                           (uint8)std::floor(jmap(HiDist, (float)theme->COLOR_GRAPH_SECONDARY.getGreen(), (float)theme->COLOR_GRAPH_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(HiDist, (float)theme->COLOR_GRAPH_SECONDARY.getBlue(), (float)theme->COLOR_GRAPH_PRIMARY.getBlue())), 0.6f));
        g.fillRect(HiBand.toFloat());
    }

    // oscilloscope if not disabled
    if (isEnabled) {
        g.setColour(theme->COLOR_GRAPH_SECONDARY);
        audioCurvePre.applyTransform(AffineTransform().translation(analysisX, analysisArea.getY()));
        g.strokePath(audioCurvePre, PathStrokeType(2));
        g.setColour(theme->COLOR_GRAPH_PRIMARY);
        audioCurvePost.applyTransform(AffineTransform().translation(analysisX, analysisArea.getY()));
        g.strokePath(audioCurvePost, PathStrokeType(2));
    }

    // band dividers
    g.setColour(theme->COLOR_GRAPH_FOREGROUND);
    Path LineLoMid;
    LineLoMid.startNewSubPath(BandLoMidX, outputMin);
    LineLoMid.lineTo(BandLoMidX, outputMax);
    g.strokePath(LineLoMid, PathStrokeType(2));
    Path LineMidHi;
    LineMidHi.startNewSubPath(BandMidHiX, outputMin);
    LineMidHi.lineTo(BandMidHiX, outputMax);
    g.strokePath(LineMidHi, PathStrokeType(2));

    // if filter params have changed, regenerate new path
    if (parametersChanged.compareAndSetBool(false, true)) {
        std::vector<double> magnitudes;
        magnitudes.resize(analysisWidth);
        const double sampleRate = processor->getSampleRate();

        for (int i = 0; i < analysisWidth; i++) {
            double freq = mapToLog10((double)i / (double)analysisWidth, 20.0, 20000.0); // X axis
            double magnitude = 1.0;                                                     // Y axis

            if (!LoCut.isBypassed<0>()) magnitude *= LoCut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!LoCut.isBypassed<1>()) magnitude *= LoCut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!LoCut.isBypassed<2>()) magnitude *= LoCut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!LoCut.isBypassed<3>()) magnitude *= LoCut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!HiCut.isBypassed<0>()) magnitude *= HiCut.get<0>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!HiCut.isBypassed<1>()) magnitude *= HiCut.get<1>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!HiCut.isBypassed<2>()) magnitude *= HiCut.get<2>().coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!HiCut.isBypassed<3>()) magnitude *= HiCut.get<3>().coefficients->getMagnitudeForFrequency(freq, sampleRate);

            if (!DSP->isBypassedLo) magnitude *= PeakLo.coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!DSP->isBypassedMid) magnitude *= PeakMid.coefficients->getMagnitudeForFrequency(freq, sampleRate);
            if (!DSP->isBypassedHi) magnitude *= PeakHi.coefficients->getMagnitudeForFrequency(freq, sampleRate);

            magnitudes[i] = Decibels::gainToDecibels(magnitude);
        }

        filterCurve.clear();
        auto map = [outputMin, outputMax](double input) { return jmap(input, -36.0, 36.0, outputMin, outputMax); };
        filterCurve.startNewSubPath(analysisArea.getX(), map(magnitudes.front()));
        for (int i = 1; i < magnitudes.size(); i++) {   // set path for every pixel
            filterCurve.lineTo(analysisArea.getX() + i, map(magnitudes[i]));
        }
    }

    // filter curve
    g.setColour(theme->COLOR_GRAPH_FOREGROUND);
    g.strokePath(filterCurve, PathStrokeType(2));
}

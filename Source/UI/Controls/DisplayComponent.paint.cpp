#include "DisplayComponent.h"
#include "../UIConsts.h"

void GnomeDistort2Controls::DisplayComponent::paint(juce::Graphics& g) {
    using namespace juce;
    using namespace GnomeDistort2UIConst;

    auto displayArea = getLocalBounds();
    auto renderArea = getRenderArea();
    auto analysisArea = getAnalysisArea();
    const int analysisX = analysisArea.getX();
    const int analysisWidth = analysisArea.getWidth();
    const int analysisHeight = analysisArea.getHeight();
    const double outputMin = analysisArea.getBottom();
    const double outputMax = analysisArea.getY();
    const int BandLoMidX = mapFromLog10(DSP->BandMidL.LowerFreqBound, 20.f, 20000.f) * analysisWidth + analysisX;
    const int BandMidHiX = mapFromLog10(DSP->BandHiL.LowerFreqBound, 20.f, 20000.f) * analysisWidth + analysisX;

    // background
    g.setColour(GnomeDistort2UIConst::COLOR_BG_VERYDARK);
    g.fillRect(displayArea.toFloat());
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

        g.setColour(Colour((uint8)COLOR_PRIMARY.getRed(),
                           (uint8)std::floor(jmap(LoDist, (float)COLOR_SECONDARY.getGreen(), (float)COLOR_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(LoDist, (float)COLOR_SECONDARY.getBlue(), (float)COLOR_PRIMARY.getBlue())), 0.6f));
        g.fillRect(LoBand.toFloat());
        g.setColour(Colour((uint8)COLOR_PRIMARY.getRed(),
                           (uint8)std::floor(jmap(MidDist, (float)COLOR_SECONDARY.getGreen(), (float)COLOR_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(MidDist, (float)COLOR_SECONDARY.getBlue(), (float)COLOR_PRIMARY.getBlue())), 0.6f));
        g.fillRect(MidBand.toFloat());
        g.setColour(Colour((uint8)COLOR_PRIMARY.getRed(),
                           (uint8)std::floor(jmap(HiDist, (float)COLOR_SECONDARY.getGreen(), (float)COLOR_PRIMARY.getGreen())),
                           (uint8)std::floor(jmap(HiDist, (float)COLOR_SECONDARY.getBlue(), (float)COLOR_PRIMARY.getBlue())), 0.6f));
        g.fillRect(HiBand.toFloat());
    }

    // oscilloscope if not disabled
    if (isEnabled) {
        g.setColour(COLOR_SECONDARY);
        audioCurvePre.applyTransform(AffineTransform().translation(analysisX, analysisArea.getY()));
        g.strokePath(audioCurvePre, PathStrokeType(2));
        g.setColour(COLOR_PRIMARY);
        audioCurvePost.applyTransform(AffineTransform().translation(analysisX, analysisArea.getY()));
        g.strokePath(audioCurvePost, PathStrokeType(2));
    }

    // band dividers
    g.setColour(Colours::white);
    Path LineLoMid;
    LineLoMid.startNewSubPath(BandLoMidX, outputMin);
    LineLoMid.lineTo(BandLoMidX, outputMax);
    g.strokePath(LineLoMid, PathStrokeType(2));
    Path LineMidHi;
    LineMidHi.startNewSubPath(BandMidHiX, outputMin);
    LineMidHi.lineTo(BandMidHiX, outputMax);
    g.strokePath(LineMidHi, PathStrokeType(2));

    // filter curve
    g.setColour(Colours::white);
    g.strokePath(filterCurve, PathStrokeType(2));
}

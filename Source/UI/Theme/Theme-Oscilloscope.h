#pragma once
#include "Theme.h"

namespace GnomeDistort2Theme {
    static const Theme getTheme_Oscilloscope() {
        Theme r;

        r.shouldDrawOverlay = true;
        r.reduced3DBorders = true;

        r.COLOR_BG = juce::Colour(145u, 136u, 127u);
        r.COLOR_BG_AREAS = juce::Colour(101u, 94u, 89u);
        r.COLOR_PRIMARY = juce::Colour(53u, 52u, 50u);
        r.COLOR_SECONDARY = juce::Colour(155u, 150u, 130u);
        r.COLOR_TEXT_PRIMARY = juce::Colour(255u, 255u, 255u);
        r.COLOR_TEXT_SECONDARY = juce::Colour(180u, 180u, 180u);
        r.COLOR_KNOB_INDICATOR_IN = juce::Colour(201u, 71u, 73u);
        r.COLOR_KNOB_INDICATOR_OUT = juce::Colour(250u, 249u, 248u);
        r.COLOR_CIRCUIT_PRIMARY = juce::Colour(39u, 33u, 27u);
        r.COLOR_GRAPH_BG = juce::Colour(41u, 50u, 33u);
        r.COLOR_GRAPH_PRIMARY = juce::Colour(72u, 185u, 107u);
        r.COLOR_GRAPH_SECONDARY = juce::Colour(121u, 176u, 174u);
        r.COLOR_BAND_LEVEL_MAX = juce::Colour(72u, 185u, 107u);
        r.COLOR_BAND_LEVEL_MIN = juce::Colour(121u, 176u, 174u);
        r.COLOR_GRAPH_FOREGROUND = juce::Colour(250u, 249u, 248u);
        r.COLOR_GRAPH_MARKINGS = juce::Colour(165u, 84u, 84u);
        r.COLOR_GRAPH_MARKINGS_DIM = juce::Colour(45u, 16u, 5u);

        r.KnobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_128_png, BinaryData::knob_overlay_128_pngSize);

        r.CornersRect = [](juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY) {
            const int left = bounds.getX();
            const int top = bounds.getY();
            const int right = left + bounds.getWidth();
            const int bottom = top + bounds.getHeight();

            const juce::Colour COLOR_ACCENT_DARK = juce::Colour(25u, 25u, 25u);
            const juce::Colour COLOR_ACCENT_MIDDARK = juce::Colour(36u, 36u, 36u);
            const juce::Colour COLOR_ACCENT_MID = juce::Colour(64u, 64u, 64u);
            const juce::Colour COLOR_ACCENT_LIGHT = juce::Colour(86u, 86u, 86u);

            juce::Path topEdge;
            topEdge.startNewSubPath(left, top);
            topEdge.lineTo(left - edgeLengthX, top - edgeLengthY);
            topEdge.lineTo(right + edgeLengthX, top - edgeLengthY);
            topEdge.lineTo(right, top);
            topEdge.closeSubPath();

            juce::Path rightEdge;
            rightEdge.startNewSubPath(right, top);
            rightEdge.lineTo(right + edgeLengthX, top - edgeLengthY);
            rightEdge.lineTo(right + edgeLengthX, bottom + edgeLengthY);
            rightEdge.lineTo(right, bottom);
            rightEdge.closeSubPath();

            juce::Path bottomEdge;
            bottomEdge.startNewSubPath(left, bottom);
            bottomEdge.lineTo(left - edgeLengthX, bottom + edgeLengthY);
            bottomEdge.lineTo(right + edgeLengthX, bottom + edgeLengthY);
            bottomEdge.lineTo(right, bottom);
            bottomEdge.closeSubPath();

            juce::Path leftEdge;
            leftEdge.startNewSubPath(left, top);
            leftEdge.lineTo(left - edgeLengthX, top - edgeLengthY);
            leftEdge.lineTo(left - edgeLengthX, bottom + edgeLengthY);
            leftEdge.lineTo(left, bottom);
            leftEdge.closeSubPath();

            g.setColour(COLOR_ACCENT_DARK);
            g.fillPath(topEdge);
            g.setColour(COLOR_ACCENT_MID);
            g.fillPath(rightEdge);
            g.setColour(COLOR_ACCENT_LIGHT);
            g.fillPath(bottomEdge);
            g.setColour(COLOR_ACCENT_MIDDARK);
            g.fillPath(leftEdge);
        };
        r.CornersKnob = [](juce::Graphics& g, juce::Rectangle<int> bounds, const int distance) {
            const juce::Colour COLOR_ACCENT_DARK_0 = juce::Colour((juce::uint8)25u, 25u, 25u, 0.f);
            const juce::Colour COLOR_ACCENT_DARK_1 = juce::Colour((juce::uint8)25u, 25u, 25u, 1.f);
            const juce::Colour COLOR_ACCENT_MID_0 = juce::Colour((juce::uint8)64u, 64u, 64u, 0.f);
            const juce::Colour COLOR_ACCENT_MID_1 = juce::Colour((juce::uint8)64u, 64u, 64u, 1.f);

            bounds.expand(distance, distance);
            juce::Path outer;
            const float distRectangleCornerToCircle = (bounds.getWidth() / 2.f) * (sqrt(2) - 1);
            outer.addEllipse(bounds.toFloat());
            juce::ColourGradient darken(
                COLOR_ACCENT_DARK_1,
                bounds.getX() + (bounds.getWidth() / 4), bounds.getY(),
                COLOR_ACCENT_DARK_0,
                bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(), true);
            g.setGradientFill(darken);
            g.fillPath(outer);
            juce::ColourGradient lighten(
                COLOR_ACCENT_MID_1,
                bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(),
                COLOR_ACCENT_MID_0,
                bounds.getX() + (bounds.getWidth() / 4), bounds.getY(), true);
            g.setGradientFill(lighten);
            g.fillPath(outer);
        };

        return r;
    };
}

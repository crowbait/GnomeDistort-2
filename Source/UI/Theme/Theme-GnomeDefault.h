#pragma once
#include "Theme.h"

namespace GnomeDistort2Theme {
    static const Theme getTheme_GnomeDefault() {
        Theme r;

        r.shouldDrawOverlay = true;
        r.reduced3DBorders = true;

        r.knobStyle = Theme::KnobLine;

        r.COLOR_BG = juce::Colour(50u, 50u, 50u);
        r.COLOR_BG_AREAS = juce::Colour(25u, 25u, 25u);
        r.COLOR_PRIMARY = juce::Colour(110u, 10u, 10u);
        r.COLOR_SECONDARY = juce::Colour(110u, 110u, 110u);
        r.COLOR_TEXT_PRIMARY = juce::Colour(255u, 255u, 255u);
        r.COLOR_TEXT_SECONDARY = juce::Colour(180u, 180u, 180u);
        r.COLOR_KNOB_INDICATOR_IN = juce::Colour(215u, 215u, 215u);
        r.COLOR_KNOB_INDICATOR_OUT = juce::Colour(25u, 25u, 25u);
        r.COLOR_CIRCUIT_PRIMARY = juce::Colour(25u, 25u, 25u);
        r.COLOR_GRAPH_BG = juce::Colour(18u, 18u, 18u);
        r.COLOR_GRAPH_PRIMARY = juce::Colour(110u, 10u, 10u);
        r.COLOR_GRAPH_SECONDARY = juce::Colour(70u, 70u, 70u);
        r.COLOR_BAND_LEVEL_MAX = juce::Colour(110u, 10u, 10u);
        r.COLOR_BAND_LEVEL_MIN = juce::Colour(70u, 70u, 70u);
        r.COLOR_GRAPH_FOREGROUND = juce::Colour(255u, 255u, 255u);
        r.COLOR_GRAPH_MARKINGS = juce::Colour(130u, 129u, 128u);
        r.COLOR_GRAPH_MARKINGS_DIM = juce::Colour(40u, 40u, 40u);

        r.KnobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_128_png, BinaryData::knob_overlay_128_pngSize);
        r.Gnome = juce::ImageCache::getFromMemory(BinaryData::gnome_dark_png, BinaryData::gnome_dark_pngSize);

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
        r.CornersKnob = [](juce::Graphics& g, const juce::Rectangle<int> bounds, const int distance) {
            const juce::Colour COLOR_ACCENT_DARK_0 = juce::Colour((juce::uint8)25u, 25u, 25u, 0.f);
            const juce::Colour COLOR_ACCENT_DARK_1 = juce::Colour((juce::uint8)25u, 25u, 25u, 1.f);
            const juce::Colour COLOR_ACCENT_MID_0 = juce::Colour((juce::uint8)64u, 64u, 64u, 0.f);
            const juce::Colour COLOR_ACCENT_MID_1 = juce::Colour((juce::uint8)64u, 64u, 64u, 1.f);

            const juce::Rectangle<int> borderBounds = bounds.expanded(distance, distance);
            juce::Path outer;
            const float distRectangleCornerToCircle = (borderBounds.getWidth() / 2.f) * (sqrt(2) - 1);
            outer.addEllipse(borderBounds.toFloat());
            juce::ColourGradient darken(
                COLOR_ACCENT_DARK_1,
                borderBounds.getX() + (borderBounds.getWidth() / 4), borderBounds.getY(),
                COLOR_ACCENT_DARK_0,
                borderBounds.getRight() - (borderBounds.getWidth() / 4), borderBounds.getBottom(), true);
            g.setGradientFill(darken);
            g.fillPath(outer);
            juce::ColourGradient lighten(
                COLOR_ACCENT_MID_1,
                borderBounds.getRight() - (borderBounds.getWidth() / 4), borderBounds.getBottom(),
                COLOR_ACCENT_MID_0,
                borderBounds.getX() + (borderBounds.getWidth() / 4), borderBounds.getY(), true);
            g.setGradientFill(lighten);
            g.fillPath(outer);
        };

        return r;
    };
}

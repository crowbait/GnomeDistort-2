#pragma once
#include "Theme.h"

namespace GnomeDistort2Theme {
    static const Theme getTheme_GnomeDefault() {
        Theme r;

        r.shouldDrawOverlay = true;
        r.reduced3DBorders = true;

        r.COLOR_BG = juce::Colour(50u, 50u, 50u);
        r.COLOR_BG_VERYDARK = juce::Colour(18u, 18u, 18u);
        r.COLOR_BG_DARK = juce::Colour(25u, 25u, 25u);
        r.COLOR_BG_MIDDARK = juce::Colour(36u, 36u, 36u);
        r.COLOR_BG_MID = juce::Colour(64u, 64u, 64u);
        r.COLOR_BG_LIGHT = juce::Colour(86u, 86u, 86u);

        r.COLOR_PRIMARY = juce::Colour(110u, 10u, 10u);
        r.COLOR_SECONDARY = juce::Colour(110u, 110u, 110u);

        r.COLOR_TEXT = juce::Colour(255u, 255u, 255u);
        r.CIRCUIT_PRIMARY = juce::Colour(25u, 25u, 25u);

        r.KnobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_128_png, BinaryData::knob_overlay_128_pngSize);

        r.CornersRect = [r](juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY) {
            int left = bounds.getX();
            int top = bounds.getY();
            int right = left + bounds.getWidth();
            int bottom = top + bounds.getHeight();

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

            g.setColour(r.COLOR_BG_DARK);
            g.fillPath(topEdge);
            g.setColour(r.COLOR_BG_MID);
            g.fillPath(rightEdge);
            g.setColour(r.COLOR_BG_LIGHT);
            g.fillPath(bottomEdge);
            g.setColour(r.COLOR_BG_MIDDARK);
            g.fillPath(leftEdge);
        };
        r.CornersKnob = [r](juce::Graphics& g, juce::Rectangle<int> bounds, const int distance) {
            bounds.expand(distance, distance);
            juce::Path outer;
            const float distRectangleCornerToCircle = (bounds.getWidth() / 2) * (sqrt(2) - 1);
            outer.addEllipse(bounds.toFloat());
            juce::ColourGradient darken(
                juce::Colour(r.COLOR_BG_DARK.getRed(), r.COLOR_BG_DARK.getGreen(), r.COLOR_BG_DARK.getBlue(), 1.f),
                bounds.getX() + (bounds.getWidth() / 4), bounds.getY(),
                juce::Colour(r.COLOR_BG_DARK.getRed(), r.COLOR_BG_DARK.getGreen(), r.COLOR_BG_DARK.getBlue(), 0.f),
                bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(), true);
            g.setGradientFill(darken);
            g.fillPath(outer);
            juce::ColourGradient lighten(
                juce::Colour(r.COLOR_BG_MID.getRed(), r.COLOR_BG_MID.getGreen(), r.COLOR_BG_MID.getBlue(), 1.f),
                bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(),
                juce::Colour(r.COLOR_BG_MID.getRed(), r.COLOR_BG_MID.getGreen(), r.COLOR_BG_MID.getBlue(), 0.f),
                bounds.getX() + (bounds.getWidth() / 4), bounds.getY(), true);
            g.setGradientFill(lighten);
            g.fillPath(outer);
        };

        return r;
    };
}

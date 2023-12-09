#pragma once
#include "Theme.h"

namespace GnomeDistort2Theme {
    static const Theme getTheme_Basalt() {
        Theme r;

        r.shouldDrawOverlay = true;
        r.reduced3DBorders = false;

        r.knobStyle = Theme::KnobLine;

        r.COLOR_BG = juce::Colour(28u, 28u, 28u);
        r.COLOR_BG_AREAS = juce::Colour(160u, 70u, 24u);
        r.COLOR_PRIMARY = juce::Colour(8u, 8u, 8u);
        r.COLOR_SECONDARY = juce::Colour(32u, 32u, 32u);
        r.COLOR_TEXT_PRIMARY = juce::Colour(255u, 255u, 255u);
        r.COLOR_TEXT_SECONDARY = juce::Colour(180u, 180u, 180u);
        r.COLOR_KNOB_INDICATOR_IN = juce::Colour(215u, 215u, 215u);
        r.COLOR_KNOB_INDICATOR_OUT = juce::Colour(25u, 25u, 25u);
        r.COLOR_CIRCUIT_PRIMARY = juce::Colour(211u, 93u, 32u);
        r.COLOR_GRAPH_BG = juce::Colour(18u, 18u, 18u);
        r.COLOR_GRAPH_PRIMARY = juce::Colour(211u, 93u, 32u);
        r.COLOR_GRAPH_SECONDARY = juce::Colour(70u, 70u, 70u);
        r.COLOR_BAND_LEVEL_MAX = juce::Colour(211u, 93u, 32u);
        r.COLOR_BAND_LEVEL_MIN = juce::Colour(70u, 70u, 70u);
        r.COLOR_GRAPH_FOREGROUND = juce::Colour(255u, 255u, 255u);
        r.COLOR_GRAPH_MARKINGS = juce::Colour(130u, 129u, 128u);
        r.COLOR_GRAPH_MARKINGS_DIM = juce::Colour(40u, 40u, 40u);

        r.KnobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_clean_128_png, BinaryData::knob_overlay_clean_128_pngSize);
        r.Gnome = juce::ImageCache::getFromMemory(BinaryData::gnome_dark_png, BinaryData::gnome_dark_pngSize);
        r.overlay = juce::ImageCache::getFromMemory(BinaryData::overlay_cracks_png, BinaryData::overlay_cracks_pngSize);

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

        // knob glow
        juce::Image knobShadow = juce::ImageCache::getFromMemory(BinaryData::knob_shadow_clean_128_png, BinaryData::knob_shadow_clean_128_pngSize);
        juce::Image knobGlow(knobShadow);
        knobGlow.duplicateIfShared();
        for (int x = 0; x < knobGlow.getWidth(); x++) {
            for (int y = 0; y < knobGlow.getHeight(); y++) {
                const juce::Colour col = knobGlow.getPixelAt(x, y);
                knobGlow.setPixelAt(x, y, juce::Colour(0.0555f, 0.74f, 0.48f, col.getAlpha()));
            }
        }
        r.internalImages = {
            knobGlow
        };
        r.CornersKnob = [r](juce::Graphics& g, const juce::Rectangle<int> bounds, const int distance) {
            const juce::Rectangle<int> borderBounds = bounds.expanded(distance, distance);
            g.drawImageWithin(r.internalImages[0], borderBounds.getX(), borderBounds.getY(), borderBounds.getWidth(), borderBounds.getHeight(), juce::RectanglePlacement::stretchToFit);
        };

        return r;
    };
}

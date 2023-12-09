#pragma once
#include "Theme.h"

namespace GnomeDistort2Theme {
    static const Theme getTheme_CleanLightRed() {
        Theme r;

        r.shouldDrawOverlay = false;
        r.reduced3DBorders = false;

        r.knobStyle = Theme::KnobTick;

        r.COLOR_BG = juce::Colour(255u, 255u, 255u);
        r.COLOR_BG_AREAS = juce::Colour(189u, 189u, 189u);
        r.COLOR_PRIMARY = juce::Colour(210u, 210u, 210u);
        r.COLOR_SECONDARY = juce::Colour(110u, 110u, 110u);
        r.COLOR_TEXT_PRIMARY = juce::Colour(0u, 0u, 0u);
        r.COLOR_TEXT_SECONDARY = juce::Colour(33u, 33u, 33u);
        r.COLOR_KNOB_INDICATOR_IN = juce::Colour(110u, 10u, 10u);
        r.COLOR_KNOB_INDICATOR_OUT = juce::Colour(36u, 36u, 36u);
        r.COLOR_CIRCUIT_PRIMARY = juce::Colour(110u, 10u, 10u);
        r.COLOR_GRAPH_BG = juce::Colour(210u, 210u, 210u);
        r.COLOR_GRAPH_PRIMARY = juce::Colour(110u, 10u, 10u);
        r.COLOR_GRAPH_SECONDARY = juce::Colour(90u, 90u, 90u);
        r.COLOR_BAND_LEVEL_MAX = juce::Colour(110u, 10u, 10u);
        r.COLOR_BAND_LEVEL_MIN = juce::Colour(90u, 90u, 90u);
        r.COLOR_GRAPH_FOREGROUND = juce::Colour(255u, 255u, 255u);
        r.COLOR_GRAPH_MARKINGS = juce::Colour(0u, 0u, 0u);
        r.COLOR_GRAPH_MARKINGS_DIM = juce::Colour(130u, 130u, 130u);
        r.COLOR_CMBBX_ARROW = juce::Colour(110u, 10u, 10u);

        r.KnobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_clean_128_png, BinaryData::knob_overlay_clean_128_pngSize);
        r.Gnome = juce::ImageCache::getFromMemory(BinaryData::gnome_dark_png, BinaryData::gnome_dark_pngSize);

        r.CornersRect = [r](juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY) {
            const juce::Rectangle<int> area = bounds.expanded(edgeLengthX, edgeLengthY);
            juce::Path areaPath;
            areaPath.addRoundedRectangle(area, 8.f);
            g.setColour(r.COLOR_BG_AREAS);
            g.fillPath(areaPath);
        };
        r.CornersKnob = [](juce::Graphics& g, const juce::Rectangle<int> bounds, const int distance) {
            const juce::Image knobShadow = juce::ImageCache::getFromMemory(BinaryData::knob_shadow_clean_128_png, BinaryData::knob_shadow_clean_128_pngSize);
            const juce::Rectangle<int> borderBounds = bounds.expanded(distance, distance);
            g.drawImageWithin(knobShadow, borderBounds.getX(), borderBounds.getY(), borderBounds.getWidth(), borderBounds.getHeight(), juce::RectanglePlacement::stretchToFit);
        };

        return r;
    };
}

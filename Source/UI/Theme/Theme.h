#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Theme {
    enum Themes {
        GnomeDefault,
        Oscilloscope
    };
    const juce::StringArray ThemeOptions = {
        "GNOME Industrial",
        "Oscilloscope"
    };

    struct Theme {
        bool shouldDrawOverlay;                 // whether to draw grundge overlay
        bool reduced3DBorders;                  // whether to exclude some controls from getting effects

        juce::Colour COLOR_BG;                  // main background of the interface
        juce::Colour COLOR_BG_AREAS;            // background of areas, such as "SMEAR" as well as borders
        juce::Colour COLOR_PRIMARY;             // color of primary knobs
        juce::Colour COLOR_SECONDARY;           // color of secondary knobs
        juce::Colour COLOR_KNOB_INDICATOR_IN;   // color of indicator line fill on knob
        juce::Colour COLOR_KNOB_INDICATOR_OUT;  // color of indicator line outline on knob
        juce::Colour COLOR_TEXT_PRIMARY;        // color of text
        juce::Colour COLOR_TEXT_SECONDARY;      // color of text that shouldn't be as important
        juce::Colour COLOR_CIRCUIT_PRIMARY;     // color of the circuit paths
        juce::Colour COLOR_GRAPH_BG;            // background color of displays and value boxes
        juce::Colour COLOR_GRAPH_PRIMARY;       // color for output graph, function graph
        juce::Colour COLOR_GRAPH_SECONDARY;     // color for input graph and value box outlines
        juce::Colour COLOR_BAND_LEVEL_MAX;      // color for band amplitudes on max distortion
        juce::Colour COLOR_BAND_LEVEL_MIN;      // color for band amplitudes on min distortion
        juce::Colour COLOR_GRAPH_FOREGROUND;    // color for most important graphs: band borders, filter curve
        juce::Colour COLOR_GRAPH_MARKINGS;      // color of display markings, eg. labels
        juce::Colour COLOR_GRAPH_MARKINGS_DIM;  // color of less important display markings, eg. grid

        juce::Image KnobOverlay;

        // effect for rectangular components
        std::function<void(juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY)> CornersRect;
        // effect for circular components
        std::function<void(juce::Graphics& g, juce::Rectangle<int> bounds, const int distance)> CornersKnob;
    };
}

#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Theme {
    enum Themes {
        GnomeDefault
    };
    const juce::StringArray ThemeOptions = {
        "Gnome Default"
    };

    struct Theme {
        bool shouldDrawOverlay;                 // whether to draw grundge overlay
        bool reduced3DBorders;                  // whether to exclude some controls from getting effects

        juce::Colour COLOR_BG;                  // main background of the interface
        juce::Colour COLOR_BG_AREAS;            // background of areas, such as "SMEAR"
        juce::Colour COLOR_PRIMARY;             // color of primary knobs
        juce::Colour COLOR_SECONDARY;           // color of secondary knobs
        juce::Colour COLOR_TEXT_PRIMARY;        // color of text
        juce::Colour COLOR_TEXT_SECONDARY;      // color of text that shouldn't be as important
        juce::Colour COLOR_CIRCUIT_PRIMARY;     // color of the circuit paths
        juce::Colour COLOR_GRAPH_BG;            // background color of displays
        juce::Colour COLOR_GRAPH_PRIMARY;       // color for output graph, function graph and band level max
        juce::Colour COLOR_GRAPH_SECONDARY;     // color for input graph and band level min
        juce::Colour COLOR_GRAPH_FOREGROUND;    // color for most important graphs: band borders, filter curve
        juce::Colour COLOR_GRAPH_MARKINGS;      // color of display markings, eg. grid

        juce::Image KnobOverlay;

        // effect for rectangular components
        std::function<void(juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY)> CornersRect;
        // effect for circular components
        std::function<void(juce::Graphics& g, juce::Rectangle<int> bounds, const int distance)> CornersKnob;
    };
}
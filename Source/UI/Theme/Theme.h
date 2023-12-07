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
        bool shouldDrawOverlay;
        bool reduced3DBorders;

        juce::Colour COLOR_BG;
        juce::Colour COLOR_BG_VERYDARK;
        juce::Colour COLOR_BG_DARK;
        juce::Colour COLOR_BG_MIDDARK;
        juce::Colour COLOR_BG_MID;
        juce::Colour COLOR_BG_LIGHT;
        juce::Colour COLOR_PRIMARY;
        juce::Colour COLOR_SECONDARY;
        juce::Colour COLOR_TEXT;
        juce::Colour CIRCUIT_PRIMARY;

        juce::Image KnobOverlay;

        std::function<void(juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY)> CornersRect;
        std::function<void(juce::Graphics& g, juce::Rectangle<int> bounds, const int distance)> CornersKnob;
    };
}

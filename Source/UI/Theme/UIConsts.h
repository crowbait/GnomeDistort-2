#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Theme {
    inline int TEXT_TINY = 8;
    inline int TEXT_SMALL = 10;
    inline int TEXT_NORMAL = 12;
    inline int SELECT_HEIGHT = 24;
    inline int COMP_PADDING = 12;
    inline int CIRCUIT_THICKNESS = 5;

    //Connect two points with different Y values, creating a step in a horizontal line.
    inline auto connectPathHorizontal = [](juce::Path& path, const juce::Point<int> start, const juce::Point<int> finish) {
        path.startNewSubPath(start.toFloat());
        const int middle = start.getX() + ((finish.getX() - start.getX()) / 2);
        path.lineTo(middle, start.getY());
        path.lineTo(middle, finish.getY());
        path.lineTo(finish.toFloat());
    };

    //Connect two points with different X values, creating a step in a vertical line.
    inline auto connectPathVertical = [](juce::Path& path, const juce::Point<int> start, const juce::Point<int> finish) {
        path.startNewSubPath(start.toFloat());
        const int middle = start.getY() + ((finish.getY() - start.getY()) / 2);
        path.lineTo(start.getX(), middle);
        path.lineTo(finish.getX(), middle);
        path.lineTo(finish.toFloat());
    };
}

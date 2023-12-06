#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2UIConst {
    inline juce::Colour COLOR_BG = juce::Colour(50u, 50u, 50u);
    inline juce::Colour COLOR_BG_VERYDARK = juce::Colour(18u, 18u, 18u);
    inline juce::Colour COLOR_BG_DARK = juce::Colour(25u, 25u, 25u);
    inline juce::Colour COLOR_BG_MIDDARK = juce::Colour(36u, 36u, 36u);
    inline juce::Colour COLOR_BG_MID = juce::Colour(64u, 64u, 64u);
    inline juce::Colour COLOR_BG_LIGHT = juce::Colour(86u, 86u, 86u);
    inline juce::Colour COLOR_PRIMARY = juce::Colour(110u, 10u, 10u);
    inline juce::Colour COLOR_SECONDARY = juce::Colour(110u, 110u, 110u);
    inline juce::Colour COLOR_TEXT = juce::Colour(255u, 255u, 255u);

    inline int TEXT_TINY = 8;
    inline int TEXT_SMALL = 10;
    inline int TEXT_NORMAL = 12;
    inline int SELECT_HEIGHT = 24;
    inline int COMP_PADDING = 12;

    inline int CIRCUIT_THICKNESS = 5;
    inline juce::Colour CIRCUIT_PRIMARY = juce::Colour(25u, 25u, 25u);

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

    //Draw 3D-ish borders around a rectangle.
    inline auto draw3DCorners = [](juce::Graphics& g, const juce::Rectangle<int> bounds, const int edgeLengthX, int edgeLengthY = -1) {
        if (edgeLengthY == -1) edgeLengthY = edgeLengthX;

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

        g.setColour(COLOR_BG_DARK);
        g.fillPath(topEdge);
        g.setColour(COLOR_BG_MID);
        g.fillPath(rightEdge);
        g.setColour(COLOR_BG_LIGHT);
        g.fillPath(bottomEdge);
        g.setColour(COLOR_BG_MIDDARK);
        g.fillPath(leftEdge);
    };

    //Draw 3D-ish round border around a knob.
    inline auto draw3Dknob = [](juce::Graphics& g, juce::Rectangle<int> bounds, const int distance) {
        bounds.expand(distance, distance);
        juce::Path outer;
        const float distRectangleCornerToCircle = (bounds.getWidth() / 2) * (sqrt(2) - 1);
        outer.addEllipse(bounds.toFloat());
        juce::ColourGradient darken(
            juce::Colour(COLOR_BG_DARK.getRed(), COLOR_BG_DARK.getGreen(), COLOR_BG_DARK.getBlue(), 1.f),
            bounds.getX() + (bounds.getWidth() / 4), bounds.getY(),
            juce::Colour(COLOR_BG_DARK.getRed(), COLOR_BG_DARK.getGreen(), COLOR_BG_DARK.getBlue(), 0.f),
            bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(), true);
        g.setGradientFill(darken);
        g.fillPath(outer);
        juce::ColourGradient lighten(
            juce::Colour(COLOR_BG_MID.getRed(), COLOR_BG_MID.getGreen(), COLOR_BG_MID.getBlue(), 1.f),
            bounds.getRight() - (bounds.getWidth() / 4), bounds.getBottom(),
            juce::Colour(COLOR_BG_MID.getRed(), COLOR_BG_MID.getGreen(), COLOR_BG_MID.getBlue(), 0.f),
            bounds.getX() + (bounds.getWidth() / 4), bounds.getY(), true);
        g.setGradientFill(lighten);
        g.fillPath(outer);
    };
}

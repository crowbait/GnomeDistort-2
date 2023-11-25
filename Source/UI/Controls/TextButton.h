#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
    struct TextButton : juce::TextButton {
        TextButton(const juce::String labelText, const juce::Colour col, const int textSize, const bool bold) :
            label(labelText), color(col), textSize(textSize), isBold(bold) {

        }

        juce::String label;
        int textSize;
        bool isBold;
        juce::Colour color;

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
            g.setColour(color);
            g.setFont(juce::Font(textSize, isBold ? juce::Font::FontStyleFlags::bold : juce::Font::FontStyleFlags::plain));
            g.drawFittedText(label, getLocalBounds(), juce::Justification::centred, 1);
        }
    };
}

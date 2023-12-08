#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct TextButton : juce::TextButton {
        TextButton(const juce::String labelText, const GnomeDistort2Theme::Theme* theme, const int textSize, const bool bold) :
            label(labelText), theme(theme), textSize(textSize), isBold(bold) {

        }

        juce::String label;
        int textSize;
        bool isBold;
        const GnomeDistort2Theme::Theme* theme;

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
            g.setColour(!shouldDrawButtonAsHighlighted ? theme->COLOR_TEXT_PRIMARY : theme->COLOR_TEXT_SECONDARY);
            g.setFont(juce::Font(textSize, isBold ? juce::Font::FontStyleFlags::bold : juce::Font::FontStyleFlags::plain));
            g.drawFittedText(label, getLocalBounds(), juce::Justification::centred, 1);
        }
    };
}

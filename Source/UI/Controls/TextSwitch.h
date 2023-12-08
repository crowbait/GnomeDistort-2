#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct TextSwitch : juce::ToggleButton {
        TextSwitch(juce::String text, int textSize, const GnomeDistort2Theme::Theme* theme) :
            labelOn(text), labelOff(text), textSize(textSize), theme(theme) {}
        TextSwitch(juce::String textOn, juce::String textOff, int textSize, const GnomeDistort2Theme::Theme* theme) :
            labelOn(textOn), labelOff(textOff), textSize(textSize), theme(theme) {}

        juce::String labelOn, labelOff;
        int textSize;
        const GnomeDistort2Theme::Theme* theme;

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
            bool isOn = getToggleState();
            g.setColour(isOn ? theme->COLOR_TEXT_PRIMARY : theme->COLOR_TEXT_SECONDARY);
            g.setFont(juce::Font(textSize, juce::Font::FontStyleFlags::bold));
            g.drawFittedText(isOn ? labelOn : labelOff, getLocalBounds(), juce::Justification::centred, 1);
        }
    };
}

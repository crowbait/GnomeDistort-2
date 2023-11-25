#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
    struct TextSwitch : juce::ToggleButton {
        TextSwitch(juce::String text, int textSize, juce::Colour colOn, juce::Colour colOff) :
            labelOn(text), labelOff(text), textSize(textSize), colorOn(colOn), colorOff(colOff) {}
        TextSwitch(juce::String textOn, juce::String textOff, int textSize, juce::Colour colOn, juce::Colour colOff) :
            labelOn(textOn), labelOff(textOff), textSize(textSize), colorOn(colOn), colorOff(colOff) {}

        juce::String labelOn, labelOff;
        int textSize;
        juce::Colour colorOn, colorOff;

        void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
            bool isOn = getToggleState();
            g.setColour(isOn ? colorOn : colorOff);
            g.setFont(juce::Font(textSize, juce::Font::FontStyleFlags::bold));
            g.drawFittedText(isOn ? labelOn : labelOff, getLocalBounds(), juce::Justification::centred, 1);
        }
    };
}

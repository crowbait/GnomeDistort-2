#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct LnFTextToggleButton : public juce::LookAndFeel_V4 {
        LnFTextToggleButton(const GnomeDistort2Theme::Theme* theme) {
            setColors(theme);
        }
        void setColors(const GnomeDistort2Theme::Theme* theme) {
            setColour(juce::TextButton::ColourIds::buttonColourId, theme->COLOR_BG_AREAS);
            setColour(juce::TextButton::ColourIds::buttonOnColourId, theme->COLOR_GRAPH_BG);
            setColour(juce::TextButton::ColourIds::textColourOffId, theme->COLOR_TEXT_PRIMARY);
            setColour(juce::TextButton::ColourIds::textColourOnId, theme->COLOR_PRIMARY);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFTextToggleButton)
    };
}

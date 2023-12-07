#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct LnFTextToggleButton : public juce::LookAndFeel_V4 {
        LnFTextToggleButton(GnomeDistort2Theme::Theme* theme) {
            setColors(theme);
        }
        void setColors(GnomeDistort2Theme::Theme* theme) {
            setColour(juce::TextButton::ColourIds::buttonColourId, theme->COLOR_BG_DARK);
            setColour(juce::TextButton::ColourIds::buttonOnColourId, theme->COLOR_BG_VERYDARK);
            setColour(juce::TextButton::ColourIds::textColourOffId, theme->COLOR_TEXT);
            setColour(juce::TextButton::ColourIds::textColourOnId, theme->COLOR_PRIMARY);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFTextToggleButton)
    };
}

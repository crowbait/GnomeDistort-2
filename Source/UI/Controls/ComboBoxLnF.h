#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct LnFComboBox : public juce::LookAndFeel_V4 {
        LnFComboBox(const GnomeDistort2Theme::Theme* theme) {
            setColors(theme);
        }
        void setColors(const GnomeDistort2Theme::Theme* theme) {
            setColour(juce::ComboBox::ColourIds::backgroundColourId, theme->COLOR_BG_DARK);
            setColour(juce::ComboBox::ColourIds::textColourId, theme->COLOR_TEXT);
            setColour(juce::ComboBox::ColourIds::outlineColourId, theme->COLOR_BG_VERYDARK);
            setColour(juce::ComboBox::ColourIds::arrowColourId, theme->COLOR_BG_LIGHT);
            setColour(juce::ComboBox::ColourIds::arrowColourId, theme->COLOR_PRIMARY);
            setColour(juce::PopupMenu::ColourIds::backgroundColourId, theme->COLOR_BG_MIDDARK);
            setColour(juce::PopupMenu::ColourIds::textColourId, theme->COLOR_TEXT);
            setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, theme->COLOR_BG_DARK);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFComboBox)
    };
}

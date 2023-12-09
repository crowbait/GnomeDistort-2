#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    struct LnFComboBox : public juce::LookAndFeel_V4 {
        LnFComboBox(const GnomeDistort2Theme::Theme* theme) {
            setColors(theme);
        }
        void setColors(const GnomeDistort2Theme::Theme* theme) {
            setColour(juce::ComboBox::ColourIds::backgroundColourId, theme->COLOR_GRAPH_BG);
            setColour(juce::ComboBox::ColourIds::textColourId, theme->COLOR_TEXT_PRIMARY);
            setColour(juce::ComboBox::ColourIds::outlineColourId, theme->COLOR_GRAPH_SECONDARY);
            setColour(juce::ComboBox::ColourIds::arrowColourId, theme->COLOR_CMBBX_ARROW);
            setColour(juce::PopupMenu::ColourIds::backgroundColourId, theme->COLOR_BG_AREAS);
            setColour(juce::PopupMenu::ColourIds::textColourId, theme->COLOR_TEXT_PRIMARY);
            setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, theme->COLOR_GRAPH_BG);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFComboBox)
    };
}

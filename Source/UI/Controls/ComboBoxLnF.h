#pragma once
#include <JuceHeader.h>
#include "../UIConsts.h"

namespace GnomeDistort2Controls {
    struct LnFComboBox : public juce::LookAndFeel_V4 {
        LnFComboBox() {
            setColour(juce::ComboBox::ColourIds::backgroundColourId, GnomeDistort2UIConst::COLOR_BG_DARK);
            setColour(juce::ComboBox::ColourIds::textColourId, GnomeDistort2UIConst::COLOR_TEXT);
            setColour(juce::ComboBox::ColourIds::outlineColourId, GnomeDistort2UIConst::COLOR_BG_VERYDARK);
            setColour(juce::ComboBox::ColourIds::arrowColourId, GnomeDistort2UIConst::COLOR_BG_LIGHT);
            setColour(juce::ComboBox::ColourIds::arrowColourId, GnomeDistort2UIConst::COLOR_PRIMARY);
            setColour(juce::PopupMenu::ColourIds::backgroundColourId, GnomeDistort2UIConst::COLOR_BG_MIDDARK);
            setColour(juce::PopupMenu::ColourIds::textColourId, GnomeDistort2UIConst::COLOR_TEXT);
            setColour(juce::PopupMenu::ColourIds::highlightedBackgroundColourId, GnomeDistort2UIConst::COLOR_BG_DARK);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFComboBox)
    };
}

#pragma once
#include <JuceHeader.h>
#include "../UIConsts.h"

namespace GnomeDistort2Controls {
    struct LnFTextToggleButton : public juce::LookAndFeel_V4 {
        LnFTextToggleButton() {
            setColour(juce::TextButton::ColourIds::buttonColourId, GnomeDistort2UIConst::COLOR_BG_DARK);
            setColour(juce::TextButton::ColourIds::buttonOnColourId, GnomeDistort2UIConst::COLOR_BG_VERYDARK);
            setColour(juce::TextButton::ColourIds::textColourOffId, GnomeDistort2UIConst::COLOR_TEXT);
            setColour(juce::TextButton::ColourIds::textColourOnId, GnomeDistort2UIConst::COLOR_PRIMARY);
        }

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFTextToggleButton)
    };
}

#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
    struct LnFInvisibleButton : public juce::LookAndFeel_V4 {
        LnFInvisibleButton() {};
        void drawDrawableButton(juce::Graphics& g, juce::DrawableButton& b, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
            // empty: don't draw
        };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LnFInvisibleButton)
    };
}

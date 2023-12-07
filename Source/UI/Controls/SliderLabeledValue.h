#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    /* takes some bounds (specifically localBounds of component), get a square region of it (so the knob is circular) and applies padding */
    inline static const juce::Rectangle<int> getSliderBounds(juce::Rectangle<int>& bounds) {
        auto size = juce::jmin(bounds.getWidth() - 12, bounds.getHeight() - 12);
        juce::Rectangle<int> r;
        r.setSize(size, size);
        r.setCentre(bounds.getCentreX(), bounds.getCentreY());
        return r;
    }

    struct LnFSliderLabeledValue : juce::LookAndFeel_V4 {
        void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                              float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                              juce::Slider&) override;
    };

    struct SliderLabeledValue : juce::Slider {
        enum DisplayMode {
            DEFAULT,
            NO_VALUE,
            VALUE_NO_DECIMALS
        };

        SliderLabeledValue(const juce::String& label, const bool smallText, const juce::Colour* color, GnomeDistort2Theme::Theme* theme, DisplayMode mode = DEFAULT) :
            juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
            label(label), isSmallText(smallText), color(color), mode(mode), theme(theme) {
            setLookAndFeel(&LNF);
        }
        ~SliderLabeledValue() {
            setLookAndFeel(nullptr);
        }

        juce::String label;
        DisplayMode mode;
        bool isSmallText;
        GnomeDistort2Theme::Theme* theme;
        const juce::Colour* color;

        void paint(juce::Graphics&) override;
        juce::String getValueDisplayString() const { return juce::String((float)getValue(), mode == VALUE_NO_DECIMALS ? 0 : 2); }

    private:
        GnomeDistort2Controls::LnFSliderLabeledValue LNF;
    };
}

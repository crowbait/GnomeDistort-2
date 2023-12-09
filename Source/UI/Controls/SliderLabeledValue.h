#pragma once
#include <JuceHeader.h>
#include "../Theme/Theme.h"

namespace GnomeDistort2Controls {
    /* takes some bounds (specifically localBounds of component), get a square region of it (so the knob is circular) and applies padding */
    inline static const juce::Rectangle<int> getSliderBounds(juce::Rectangle<int> bounds) {
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
        enum Color {
            PRIMARY,
            SECONDARY
        };

        SliderLabeledValue(const juce::String& label, const bool smallText, const GnomeDistort2Theme::Theme* theme, Color color, DisplayMode mode = DEFAULT) :
            juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
            label(label), isSmallText(smallText), mode(mode), color(color), theme(theme) {
            setLookAndFeel(&LNF);
        }
        ~SliderLabeledValue() {
            setLookAndFeel(nullptr);
        }

        juce::String label;
        DisplayMode mode;
        Color color;
        bool isSmallText;
        const GnomeDistort2Theme::Theme* theme;

        void paint(juce::Graphics&) override;
        juce::String getValueDisplayString() const { return juce::String((float)getValue(), mode == VALUE_NO_DECIMALS ? 0 : 2); }

    private:
        GnomeDistort2Controls::LnFSliderLabeledValue LNF;
    };
}

#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
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

        SliderLabeledValue(const juce::String& label, const bool smallText, juce::Image* knobOverlay, juce::Colour* color, DisplayMode mode = DEFAULT) :
            juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
            label(label), isSmallText(smallText), overlay(knobOverlay), color(color), mode(mode) {
            setLookAndFeel(&LNF);
        }
        ~SliderLabeledValue() {
            setLookAndFeel(nullptr);
        }

        juce::String label;
        DisplayMode mode;
        bool isSmallText;
        juce::Image* overlay;
        juce::Colour* color;

        void paint(juce::Graphics&) override;
        juce::Rectangle<int> getSliderBounds(juce::Rectangle<int>&) const; // const at END of function declares that this function MUST not change class member variables
        juce::String getValueDisplayString() const { return juce::String((float)getValue(), mode == VALUE_NO_DECIMALS ? 0 : 2); }

    private:
        GnomeDistort2Controls::LnFSliderLabeledValue LNF;
    };
}

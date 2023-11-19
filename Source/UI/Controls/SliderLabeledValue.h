#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Controls {
    struct LnFSliderLabeledValue : juce::LookAndFeel_V4 {
        void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                              float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                              juce::Slider&) override;
    };

    struct SliderLabeledValue : juce::Slider {
        SliderLabeledValue(const juce::String& label, const bool smallText, const bool showDecimals, juce::Image& knobOverlay) :
            juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
            label(label), isSmallText(smallText), isShowDecimals(showDecimals), overlay(knobOverlay) {
            setLookAndFeel(&LNF);
        }
        ~SliderLabeledValue() {
            setLookAndFeel(nullptr);
        }

        juce::String label;
        bool isSmallText, isShowDecimals;
        juce::Image& overlay;

        void paint(juce::Graphics&) override;
        juce::Rectangle<int> getSliderBounds(juce::Rectangle<int>&) const; // const at END of function declares that this function MUST not change class member variables
        juce::String getValueDisplayString() const { return juce::String((float)getValue(), isShowDecimals ? 2 : 0); }

    private:
        LnFSliderLabeledValue LNF;
    };
}

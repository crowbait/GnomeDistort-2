#include "SliderLabeledValue.h"
#include "../Theme/UIConsts.h"

void GnomeDistort2Controls::LnFSliderLabeledValue::drawRotarySlider(juce::Graphics& g,
                                                                    int x, int y, int width, int height,
                                                                    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    using namespace juce;
    Rectangle<float> bounds = Rectangle<float>(x, y, width, height);
    Point<float> center = bounds.getCentre();

    if (auto* sldr = dynamic_cast<SliderLabeledValue*>(&slider)) {
        const int textHeight = sldr->isSmallText ? GnomeDistort2Theme::TEXT_SMALL : GnomeDistort2Theme::TEXT_NORMAL;

        // knob body
        switch (sldr->color) {
            case SliderLabeledValue::PRIMARY: g.setColour(sldr->theme->COLOR_PRIMARY); break;
            case SliderLabeledValue::SECONDARY: g.setColour(sldr->theme->COLOR_SECONDARY); break;
        }
        g.fillEllipse(bounds.toFloat());

        g.drawImage(sldr->theme->KnobOverlay, bounds, RectanglePlacement::stretchToFit);

        // label
        Rectangle<float> labelboxRect;
        Path labelboxP;
        String labeltext = sldr->label;
        g.setFont(textHeight);
        labelboxRect.setSize(g.getCurrentFont().getStringWidth(labeltext) + (sldr->isSmallText ? 2 : 4), textHeight + (sldr->isSmallText ? 1 : 2));
        labelboxRect.setCentre(center.getX(), center.getY() - bounds.getHeight() * 0.2f);
        labelboxP.addRoundedRectangle(labelboxRect, 2.f);
        g.setColour(sldr->theme->COLOR_GRAPH_SECONDARY);
        g.strokePath(labelboxP, PathStrokeType(1));
        g.setColour(sldr->theme->COLOR_GRAPH_BG);
        g.fillPath(labelboxP);

        g.setColour(sldr->theme->COLOR_TEXT_SECONDARY);
        g.drawFittedText(labeltext, labelboxRect.toNearestInt(), Justification::centred, 1);

        // value box
        if (sldr->mode != GnomeDistort2Controls::SliderLabeledValue::NO_VALUE) {
            Rectangle<float> valueboxRect;
            Path valueboxP;
            auto strWidthValue = g.getCurrentFont().getStringWidth("12345"); // 5 digits for all-same value boxes
            valueboxRect.setSize(strWidthValue + (sldr->isSmallText ? 2 : 4), textHeight + (sldr->isSmallText ? 1 : 2));
            valueboxRect.setCentre(center.getX(), center.getY() + bounds.getHeight() * 0.25f);
            valueboxP.addRoundedRectangle(valueboxRect, 2.f);
            g.setColour(sldr->theme->COLOR_GRAPH_SECONDARY);
            g.strokePath(valueboxP, PathStrokeType(1));
            g.setColour(sldr->theme->COLOR_GRAPH_BG);
            g.fillPath(valueboxP);

            g.setColour(sldr->theme->COLOR_TEXT_SECONDARY);
            g.drawFittedText(sldr->getValueDisplayString(), valueboxRect.toNearestInt(), Justification::centred, 1);
        }

        // indicator
        switch (sldr->theme->knobStyle) {
            case GnomeDistort2Theme::Theme::KnobLine: {
                Rectangle<float> indicatorRect;
                Path indicatorPath;
                indicatorRect.setLeft(center.getX() - 2);
                indicatorRect.setRight(center.getX() + 2);
                indicatorRect.setTop(bounds.getY());
                indicatorRect.setBottom(center.getY());
                indicatorPath.addRectangle(indicatorRect);
                jassert(rotaryStartAngle < rotaryEndAngle);
                float sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
                indicatorPath.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));
                g.setColour(sldr->theme->COLOR_KNOB_INDICATOR_IN);
                g.fillPath(indicatorPath);
                g.setColour(sldr->theme->COLOR_KNOB_INDICATOR_OUT);
                g.strokePath(indicatorPath, PathStrokeType(1));
            } break;
            case GnomeDistort2Theme::Theme::KnobTick: {
                Rectangle<float> indicatorRect;
                Path indicatorPath;
                indicatorRect.setLeft(center.getX() - 2);
                indicatorRect.setRight(center.getX() + 2);
                indicatorRect.setTop(bounds.getY());
                indicatorRect.setBottom(bounds.getY() + ((center.getY() - bounds.getY()) / 4));
                indicatorPath.addRectangle(indicatorRect);
                jassert(rotaryStartAngle < rotaryEndAngle);
                float sliderAngRad = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
                indicatorPath.applyTransform(AffineTransform().rotated(sliderAngRad, center.getX(), center.getY()));
                g.setColour(sldr->theme->COLOR_KNOB_INDICATOR_IN);
                g.fillPath(indicatorPath);
                g.setColour(sldr->theme->COLOR_KNOB_INDICATOR_OUT);
                g.strokePath(indicatorPath, PathStrokeType(1));
            } break;
        }
    } else jassertfalse;    // catch calls to this function which are not from a matching slider
}

//==============================================================================

void GnomeDistort2Controls::SliderLabeledValue::paint(juce::Graphics& g) {
    using namespace juce;
    Rectangle<int> bounds = getLocalBounds();
    Rectangle<int> sliderBounds = getSliderBounds(bounds);
    constexpr float minAngle = degreesToRadians(-135.f);
    constexpr float maxAngle = degreesToRadians(135.f);

    // DEBUG: SHOW BOUNDS
    // g.setColour(Colours::yellowgreen);
    // g.drawRect(getLocalBounds());
    // g.setColour(Colours::aqua);
    // g.drawRect(sliderBounds);

    getLookAndFeel().drawRotarySlider(g,
                                      sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight(),
                                      valueToProportionOfLength(getValue()), minAngle, maxAngle, *this);
}

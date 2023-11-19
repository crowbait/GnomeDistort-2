#include "SliderLabeledValue.h"
#include "../UIConsts.h"

void GnomeDistort2Controls::LnFSliderLabeledValue::drawRotarySlider(juce::Graphics& g,
                                                                    int x, int y, int width, int height,
                                                                    float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    using namespace juce;
    Rectangle<int> bounds = Rectangle<int>(x, y, width, height);
    Point<int> center = bounds.getCentre();

    // knob body
    g.setColour(GnomeDistort2UIConst::COLOR_PRIMARY);
    g.fillEllipse(bounds.toFloat());

    if (auto* sldr = dynamic_cast<SliderLabeledValue*>(&slider)) {

    } else jassertfalse;    // catch calls to this function which are not from a matching slider
}

//==============================================================================

void GnomeDistort2Controls::SliderLabeledValue::paint(juce::Graphics& g) {
    using namespace juce;

    Rectangle<int> sliderBounds = getSliderBounds(getLocalBounds());
    constexpr float minAngle = degreesToRadians(-135.f);
    constexpr float maxAngle = degreesToRadians(135.f);

    // DEBUG: SHOW BOUNDS
    g.setColour(Colours::yellowgreen);
    g.drawRect(getLocalBounds());
    g.setColour(Colours::aqua);
    g.drawRect(sliderBounds);

    getLookAndFeel().drawRotarySlider(g,
                                      sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(), sliderBounds.getHeight(),
                                      valueToProportionOfLength(getValue()), minAngle, maxAngle, *this);
}

/* takes some bounds (specifically localBounds of component), get a square region of it (so the knob is circular) and applies padding */
juce::Rectangle<int> GnomeDistort2Controls::SliderLabeledValue::getSliderBounds(juce::Rectangle<int>& bounds) const {
    auto size = juce::jmin(bounds.getWidth() - 12, bounds.getHeight() - 12);
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), bounds.getCentreY());
    return r;
}

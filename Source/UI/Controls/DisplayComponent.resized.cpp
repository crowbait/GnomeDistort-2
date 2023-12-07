#include "DisplayComponent.h"
#include "../Theme/UIConsts.h"

void GnomeDistort2Controls::DisplayComponent::resized() {
    using namespace juce;
    using namespace GnomeDistort2Theme;

    auto bounds = getAnalysisArea();
    int left = bounds.getX();
    int right = bounds.getRight();
    int top = bounds.getY();
    int bottom = bounds.getBottom();
    int width = bounds.getWidth();
    int height = bounds.getHeight();
    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);
    Graphics g(background);

    g.setColour(theme->COLOR_BG_VERYDARK);
    g.fillRect(getLocalBounds().toFloat());

    g.setColour(Colours::dimgrey);
    g.setFont(TEXT_TINY);

    // draw frequencies
    Array<float> freqs{
        30, 50, 100,
        200, 500, 1000,
        2000, 5000, 10000, 20000
    };
    Array<float> xs;
    for (float f : freqs) {
        float normalizedX = mapFromLog10(f, 20.f, 20000.f);  // same as in drawing filter response curve
        xs.add(left + width * normalizedX);
    }
    g.setColour(theme->COLOR_BG_MIDDARK);
    for (float x : xs) {
        g.drawVerticalLine(x, top, bottom);
    }
    g.setColour(Colours::dimgrey);
    for (int i = 0; i < freqs.size(); i++) {
        float f = freqs[i];
        float x = xs[i];

        bool addK = false;
        String str;
        if (f > 999.f) {
            addK = true;
            f /= 1000.f;
        }
        str << f;
        if (addK) str << "k";
        str << "Hz";

        Rectangle<int> r;
        r.setSize(g.getCurrentFont().getStringWidth(str), TEXT_TINY);
        r.setCentre(x - 4, 0);
        r.setY(1);
        g.drawFittedText(str, r, Justification::centred, 1);
    }

    // draw gains
    Array<float> gains{
        -36, -24, -12, 0, 12, 24, 36
    };
    Array<float> ys;
    for (float gdB : gains) {
        auto normalizedY = jmap(gdB, -36.f, 36.f, (float)bottom, (float)top);
        ys.add(normalizedY);
        g.setColour(gdB == 0.f ? Colours::dimgrey : theme->COLOR_BG_MIDDARK);
        g.drawHorizontalLine(normalizedY, left, right);

        String str;
        str << gdB;
        str << "dB";
        Rectangle<int> r;
        r.setSize(g.getCurrentFont().getStringWidth(str), GnomeDistort2Theme::TEXT_TINY);
        r.setCentre(0, normalizedY);
        r.setX(2);
        g.setColour(Colours::dimgrey);
        g.drawFittedText(str, r, Justification::centred, 1);
    }

    g.setColour(theme->COLOR_PRIMARY);
    g.drawRoundedRectangle(bounds.toFloat(), 2.f, 1.f);
}

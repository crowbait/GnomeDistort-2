#include "BandControls.h"
#include "UIConsts.h"

juce::Image BandControls::paintBackground() {
    using namespace juce;
    using namespace GnomeDistort2UIConst;

    Image background(Image::PixelFormat::ARGB, getWidth(), getHeight(), true);
    Graphics g(background);



    return background;
}

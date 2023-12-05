#include "PluginEditor.h"

void GnomeDistort2AudioProcessorEditor::paintBackground() {
    using namespace juce;

    const int width = getWidth();
    const int height = getHeight();

    background = Image(Image::PixelFormat::RGB, width, height, false);
    Graphics g(background);

    g.setColour(GnomeDistort2UIConst::COLOR_BG);
    g.fillRect(getLocalBounds());
    g.drawImageWithin(ImageCache::getFromMemory(BinaryData::grundge_overlay_png, BinaryData::grundge_overlay_pngSize),
                      0, 0, width, height, RectanglePlacement::fillDestination);
    Image gnome = ImageCache::getFromMemory(BinaryData::gnome_dark_png, BinaryData::gnome_dark_pngSize);
    g.drawImageAt(gnome, width - gnome.getWidth(), height - gnome.getHeight());

    // draw circuits
#pragma region circuit
    Path circuit;

    // connect two points with different Y values, creating a step in a horizontal line
    auto connectHorizontal = [](Path& path, const Point<int> start, const Point<int> finish) {
        path.startNewSubPath(start.toFloat());
        const int middle = start.getX() + ((finish.getX() - start.getX()) / 2);
        path.lineTo(middle, start.getY());
        path.lineTo(middle, finish.getY());
        path.lineTo(finish.toFloat());
    };
    // connect two points with different X values, creating a step in a vertical line
    auto connectVertical = [](Path& path, const Point<int> start, const Point<int> finish) {
        path.startNewSubPath(start.toFloat());
        const int middle = start.getY() + ((finish.getY() - start.getY()) / 2);
        path.lineTo(start.getX(), middle);
        path.lineTo(finish.getX(), middle);
        path.lineTo(finish.toFloat());
    };

    // start and pre-band controls
#pragma region circuit_start+preBand
    circuit.startNewSubPath(0, DisplayArea.getBounds().getCentreY());
    circuit.lineTo(DisplayArea.getX(), DisplayArea.getBounds().getCentreY());

    const Point<int> PreBandCorner = PreBandControl.getPosition();

    circuit.startNewSubPath(PreBandControl.getBounds().getCentreX(), DisplayArea.getBottom());
    circuit.lineTo(PreBandControl.getBounds().getCentreX(), PreBandControl.BandLoMidSlider.getBounds().getCentreY() + PreBandCorner.getY());
    circuit.startNewSubPath((PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner).toFloat());
    circuit.lineTo((PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner).toFloat());

    // band sliders to cutoff frequencies
    connectHorizontal(circuit, PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner, PreBandControl.LoCutSlider.getBounds().getCentre() + PreBandCorner);
    connectHorizontal(circuit, PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner, PreBandControl.HiCutSlider.getBounds().getCentre() + PreBandCorner);

    // lo/hi cutoff to bands
    connectVertical(circuit, PreBandControl.LoCutSlider.getBounds().getCentre() + PreBandCorner, Point<int>(BandControlsLo.getBounds().getCentreX(), BandControlsLo.getY()));
    connectVertical(circuit, PreBandControl.HiCutSlider.getBounds().getCentre() + PreBandCorner, Point<int>(BandControlsHi.getBounds().getCentreX(), BandControlsHi.getY()));
    // band sliders to mid band
    const int midYBandSlidersMidBand = PreBandCorner.getY() + PreBandControl.BandLoMidSlider.getBottom() + GnomeDistort2UIConst::COMP_PADDING;
    circuit.startNewSubPath((PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner).toFloat());
    circuit.lineTo((PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner).getX(), midYBandSlidersMidBand);
    circuit.lineTo(PreBandControl.getBounds().getCentreX(), midYBandSlidersMidBand);
    circuit.lineTo(BandControlsMid.getBounds().getCentreX(), BandControlsMid.getY());
    circuit.startNewSubPath((PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner).toFloat());
    circuit.lineTo((PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner).getX(), midYBandSlidersMidBand);
    circuit.lineTo(PreBandControl.getBounds().getCentreX(), midYBandSlidersMidBand);
#pragma endregion

    // band controls
#pragma region circuit_bands
    std::vector<BandControls*> bands = {
        &BandControlsLo,
        &BandControlsMid,
        &BandControlsHi
    };
    for (const auto band : bands) {
        //g.drawImageAt(band->drawBackground(g), band->getX(), band->getY(), false);
        circuit.startNewSubPath((band->PostGainSlider.getBounds().getCentre() + band->getPosition()).toFloat());
        circuit.lineTo((band->PostGainSlider.getBounds().getCentre() + band->getPosition()).getX(), band->getBottom());
    }
#pragma endregion

    // band controls to post band to end
#pragma region circuit_postband
    circuit.startNewSubPath(BandControlsLo.PostGainSlider.getBounds().getCentre().getX() + BandControlsLo.getX(), BandControlsLo.getBottom());
    circuit.lineTo(PostBandControl.WaveshapeAmtSlider.getBounds().getCentre().getX() + PostBandControl.getX(), BandControlsLo.getBottom());
    circuit.lineTo(MixSlider.getBounds().getCentre().toFloat());
    circuit.lineTo(getRight(), MixSlider.getBounds().getCentre().getY());
#pragma endregion

    g.setColour(GnomeDistort2UIConst::COLOR_BG_VERYDARK);
    g.strokePath(circuit, PathStrokeType(GnomeDistort2UIConst::CIRCUIT_THICKNESS));
#pragma endregion


}

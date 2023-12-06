#include "PluginEditor.h"

void GnomeDistort2AudioProcessorEditor::paintBackground() {
    using namespace juce;
    using namespace GnomeDistort2UIConst;

    const int width = getWidth();
    const int height = getHeight();

    const Point<int> PreBandCorner = PreBandControl.getPosition();
    const Point<int> PostBandCorner = PostBandControl.getPosition();
    std::vector<BandControls*> bands = {
        &BandControlsLo,
        &BandControlsMid,
        &BandControlsHi
    };

    background = Image(Image::PixelFormat::RGB, width, height, false);
    Graphics g(background);

    g.setColour(COLOR_BG);
    g.fillRect(getLocalBounds());
    g.drawImageWithin(ImageCache::getFromMemory(BinaryData::grundge_overlay_png, BinaryData::grundge_overlay_pngSize),
                      0, 0, width, height, RectanglePlacement::fillDestination);
    Image gnome = ImageCache::getFromMemory(BinaryData::gnome_dark_png, BinaryData::gnome_dark_pngSize);
    g.drawImageAt(gnome, width - gnome.getWidth(), height - gnome.getHeight());

    // draw circuits
#pragma region circuit
    Path circuit;

    // start and pre-band controls
#pragma region circuit_start+preBand
    circuit.startNewSubPath(0, DisplayArea.getBounds().getCentreY());
    circuit.lineTo(DisplayArea.getX(), DisplayArea.getBounds().getCentreY());

    circuit.startNewSubPath(PreBandControl.getBounds().getCentreX(), DisplayArea.getBottom());
    circuit.lineTo(PreBandControl.getBounds().getCentreX(), PreBandControl.BandLoMidSlider.getBounds().getCentreY() + PreBandCorner.getY());
    circuit.startNewSubPath((PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner).toFloat());
    circuit.lineTo((PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner).toFloat());

    // band sliders to cutoff frequencies
    connectPathHorizontal(circuit, PreBandControl.BandLoMidSlider.getBounds().getCentre() + PreBandCorner, PreBandControl.LoCutSlider.getBounds().getCentre() + PreBandCorner);
    connectPathHorizontal(circuit, PreBandControl.BandMidHiSlider.getBounds().getCentre() + PreBandCorner, PreBandControl.HiCutSlider.getBounds().getCentre() + PreBandCorner);

    // lo/hi cutoff to bands
    connectPathVertical(circuit, PreBandControl.LoCutSlider.getBounds().getCentre() + PreBandCorner, Point<int>(BandControlsLo.getBounds().getCentreX(), BandControlsLo.getY()));
    connectPathVertical(circuit, PreBandControl.HiCutSlider.getBounds().getCentre() + PreBandCorner, Point<int>(BandControlsHi.getBounds().getCentreX(), BandControlsHi.getY()));
    // band sliders to mid band
    const int midYBandSlidersMidBand = PreBandCorner.getY() + PreBandControl.BandLoMidSlider.getBottom() + COMP_PADDING;
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
    for (const auto band : bands) {
        const Point<int> bandCorner = band->getPosition();

        // border
        Path border;
        Rectangle<int> borderBox = band->getBounds().expanded(8, 0);
        border.addRoundedRectangle(borderBox, 8.f);
        g.setColour(CIRCUIT_PRIMARY);
        g.strokePath(border, PathStrokeType(3));

        // boxes
        Path box;
        const Rectangle<int> smearBox(band->SmearAmtSlider.getX() + bandCorner.getX(),
                                      band->SmearAmtSlider.getY() + bandCorner.getY() + 4,
                                      band->SmearLengthSlider.getRight() - band->SmearAmtSlider.getX(),
                                      band->SmearLengthSlider.getBottom() - band->SmearAmtSlider.getY() - 8);
        box.addRoundedRectangle(smearBox, 4.f);
        g.fillPath(box);

        // labels
        g.setFont(TEXT_NORMAL);
        g.setColour(juce::Colours::lightgrey);
        g.drawText("SMEAR",
                   smearBox.getX() + 6,
                   smearBox.getBottom() - TEXT_NORMAL - 4,
                   g.getCurrentFont().getStringWidth("SMEAR"),
                   TEXT_NORMAL, Justification::centred);

        g.setColour(CIRCUIT_PRIMARY);

        // border to pre-gain
        circuit.startNewSubPath(band->PeakFreqSlider.getBounds().getCentreX() + bandCorner.getX(), borderBox.getY());
        circuit.lineTo((band->PeakFreqSlider.getBounds().getCentre() + bandCorner).toFloat());
        connectPathVertical(circuit, band->PeakFreqSlider.getBounds().getCentre() + bandCorner, band->PreGainSlider.getBounds().getCentre() + bandCorner);
        // peak gain to Q
        circuit.startNewSubPath((band->PeakGainSlider.getBounds().getCentre() + bandCorner).toFloat());
        circuit.lineTo((band->PeakQSlider.getBounds().getCentre() + bandCorner).toFloat());
        // pre-gain to smear to dist to gain
        circuit.startNewSubPath((band->PreGainSlider.getBounds().getCentre() + bandCorner).toFloat());
        circuit.lineTo(smearBox.getX(), band->PreGainSlider.getBounds().getCentreY() + bandCorner.getY());
        circuit.startNewSubPath(band->Display.getBounds().getCentreX() + bandCorner.getX(), smearBox.getBottom());
        circuit.lineTo((band->Display.getBounds().getCentre() + bandCorner).toFloat());
        circuit.lineTo((band->WaveshapeAmtSlider.getBounds().getCentre() + bandCorner).toFloat());
        connectPathVertical(circuit, band->WaveshapeAmtSlider.getBounds().getCentre() + bandCorner, band->PostGainSlider.getBounds().getCentre() + bandCorner);

        circuit.startNewSubPath((band->PostGainSlider.getBounds().getCentre() + band->getPosition()).toFloat());
        circuit.lineTo((band->PostGainSlider.getBounds().getCentre() + band->getPosition()).getX(), band->getBottom() + (COMP_PADDING / 2));
    }
#pragma endregion

    // band controls to post band to end
#pragma region circuit_postband
    circuit.startNewSubPath(BandControlsLo.PostGainSlider.getBounds().getCentre().getX() + BandControlsLo.getX(), BandControlsLo.getBottom() + (COMP_PADDING / 2));
    circuit.lineTo(PostBandControl.WaveshapeAmtSlider.getBounds().getCentre().getX() + PostBandControl.getX(), BandControlsLo.getBottom() + (COMP_PADDING / 2));
    circuit.lineTo(MixSlider.getBounds().getCentre().toFloat());
    circuit.lineTo(getRight(), MixSlider.getBounds().getCentre().getY());
#pragma endregion

    g.setColour(CIRCUIT_PRIMARY);
    g.strokePath(circuit, PathStrokeType(CIRCUIT_THICKNESS));
#pragma endregion

    // draw 3D borders
#pragma region 3DBorders
    draw3DCorners(g, DisplayArea.getBounds(), COMP_PADDING * 1.5f, COMP_PADDING);
    draw3Dknob(g, PreBandControl.LoCutSlider.getSliderBounds(PreBandControl.LoCutSlider.getBounds()) + PreBandCorner, COMP_PADDING / 3);
    draw3DCorners(g, PreBandControl.LoCutSlopeSelect.getBounds() + PreBandCorner, COMP_PADDING / 3);
    draw3Dknob(g, PreBandControl.BandLoMidSlider.getSliderBounds(PreBandControl.BandLoMidSlider.getBounds()) + PreBandCorner, COMP_PADDING / 2);
    draw3Dknob(g, PreBandControl.BandMidHiSlider.getSliderBounds(PreBandControl.BandMidHiSlider.getBounds()) + PreBandCorner, COMP_PADDING / 2);
    draw3Dknob(g, PreBandControl.HiCutSlider.getSliderBounds(PreBandControl.HiCutSlider.getBounds()) + PreBandCorner, COMP_PADDING / 3);
    draw3DCorners(g, PreBandControl.HiCutSlopeSelect.getBounds() + PreBandCorner, COMP_PADDING / 3);

    draw3Dknob(g, MixSlider.getSliderBounds(MixSlider.getBounds()), COMP_PADDING / 2);
    draw3Dknob(g, PostGainSlider.getSliderBounds(PostGainSlider.getBounds()), COMP_PADDING / 2);
    draw3Dknob(g, PostBandControl.WaveshapeAmtSlider.getSliderBounds(PostBandControl.WaveshapeAmtSlider.getBounds()) + PostBandCorner, COMP_PADDING / 2);
    draw3DCorners(g, PostBandControl.WaveshapeFuncSelect.getBounds() + PostBandCorner, COMP_PADDING / 3);
#pragma endregion

    // write version info
    String version = "v.";
    version << ProjectInfo::versionString;
    g.setFont(TEXT_NORMAL);
    g.setColour(juce::Colours::lightgrey);
    Rectangle<int> versionBox = Rectangle<int>(
        COMP_PADDING,
        getHeight() - TEXT_NORMAL - 8,
        g.getCurrentFont().getStringWidth(version),
        TEXT_NORMAL + 8
    );
    g.drawFittedText(version, versionBox, Justification::centred, 1);
}

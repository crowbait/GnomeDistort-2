#include "PluginEditor.h"
#include "UI/Theme/UIConsts.h"

void GnomeDistort2AudioProcessorEditor::paintBackground() {
    using namespace juce;
    using namespace GnomeDistort2Theme;
    using namespace GnomeDistort2Controls;

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

    g.setColour(theme.COLOR_BG);
    g.fillRect(getLocalBounds());

    Rectangle<int> logoBox(SwitchDisplayHQButton.getRight(), 8, 1, 1);
    logoBox.setBottom(SwitchDisplayHQButton.getBottom() - 8);
    logoBox.setRight(LinkGithubButton.getX());
    g.drawImageWithin(ImageCache::getFromMemory(BinaryData::logo_small_png, BinaryData::logo_small_pngSize),
                      logoBox.getX(), logoBox.getY(), logoBox.getWidth(), logoBox.getHeight(),
                      RectanglePlacement::centred);
    if (theme.shouldDrawOverlay) g.drawImageWithin(ImageCache::getFromMemory(BinaryData::grundge_overlay_png, BinaryData::grundge_overlay_pngSize),
                                                   0, 0, width, height, RectanglePlacement::fillDestination);
    g.drawImageAt(theme.Gnome, width - theme.Gnome.getWidth(), height - theme.Gnome.getHeight());

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
        g.setColour(theme.COLOR_BG_AREAS);
        g.strokePath(border, PathStrokeType(3));

        // boxes
        Path box;
        const Rectangle<int> smearBox(band->SmearAmtSlider.getX() + bandCorner.getX(),
                                      band->SmearAmtSlider.getY() + bandCorner.getY() + 4,
                                      band->SmearLengthSlider.getRight() - band->SmearAmtSlider.getX() - 2,
                                      band->SmearLengthSlider.getBottom() - band->SmearAmtSlider.getY() - 8);
        box.addRoundedRectangle(smearBox, 4.f);
        g.setColour(theme.COLOR_BG_AREAS);
        g.fillPath(box);

        // labels
        g.setFont(TEXT_NORMAL);
        g.setColour(theme.COLOR_TEXT_SECONDARY);
        g.drawText("SMEAR",
                   smearBox.getX() + 6,
                   smearBox.getBottom() - TEXT_NORMAL - 4,
                   g.getCurrentFont().getStringWidth("SMEAR"),
                   TEXT_NORMAL, Justification::centred);

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
        circuit.lineTo((band->PostGainSlider.getBounds().getCentre() + band->getPosition()).getX(), band->getBottom() + (COMP_PADDING / 2) + (CIRCUIT_THICKNESS / 2));
    }
#pragma endregion

    // band controls to post band to end
#pragma region circuit_postband
    circuit.startNewSubPath(BandControlsLo.PostGainSlider.getBounds().getCentre().getX() + BandControlsLo.getX(), BandControlsLo.getBottom() + (COMP_PADDING / 2));
    circuit.lineTo(PostBandControl.WaveshapeAmtSlider.getBounds().getCentre().getX() + PostBandControl.getX(), BandControlsLo.getBottom() + (COMP_PADDING / 2));
    circuit.lineTo(MixSlider.getBounds().getCentre().toFloat());
    circuit.lineTo(getRight(), MixSlider.getBounds().getCentre().getY());
#pragma endregion

    g.setColour(theme.COLOR_CIRCUIT_PRIMARY);
    g.strokePath(circuit, PathStrokeType(CIRCUIT_THICKNESS));
#pragma endregion

    // draw 3D borders
#pragma region 3DBorders
    theme.CornersRect(g, DisplayArea.getBounds(), COMP_PADDING * 1.5f, COMP_PADDING);
    theme.CornersKnob(g, getSliderBounds(PreBandControl.LoCutSlider.getBounds()) + PreBandCorner, COMP_PADDING / 3);
    theme.CornersRect(g, PreBandControl.LoCutSlopeSelect.getBounds() + PreBandCorner, COMP_PADDING / 3, COMP_PADDING / 3);
    theme.CornersKnob(g, getSliderBounds(PreBandControl.BandLoMidSlider.getBounds()) + PreBandCorner, COMP_PADDING / 2);
    theme.CornersKnob(g, getSliderBounds(PreBandControl.BandMidHiSlider.getBounds()) + PreBandCorner, COMP_PADDING / 2);
    theme.CornersKnob(g, getSliderBounds(PreBandControl.HiCutSlider.getBounds()) + PreBandCorner, COMP_PADDING / 3);
    theme.CornersRect(g, PreBandControl.HiCutSlopeSelect.getBounds() + PreBandCorner, COMP_PADDING / 3, COMP_PADDING / 3);

    theme.CornersKnob(g, getSliderBounds(MixSlider.getBounds()), COMP_PADDING / 2);
    theme.CornersKnob(g, getSliderBounds(PostGainSlider.getBounds()), COMP_PADDING / 2);
    theme.CornersKnob(g, getSliderBounds(PostBandControl.WaveshapeAmtSlider.getBounds()) + PostBandCorner, COMP_PADDING / 2);
    theme.CornersRect(g, PostBandControl.WaveshapeFuncSelect.getBounds() + PostBandCorner, COMP_PADDING / 3, COMP_PADDING / 3);

    for (auto band : bands) {
        Point<int> bandCorner = band->getPosition();
        theme.CornersKnob(g, getSliderBounds(band->PreGainSlider.getBounds()) + bandCorner, COMP_PADDING / 3);
        theme.CornersKnob(g, getSliderBounds(band->WaveshapeAmtSlider.getBounds()) + bandCorner, COMP_PADDING / 2);
        theme.CornersRect(g, band->Display.getBounds() + bandCorner, COMP_PADDING / 3, COMP_PADDING / 3);
        theme.CornersRect(g, band->WaveshapeFuncSelect.getBounds() + bandCorner, COMP_PADDING / 3, COMP_PADDING / 3);
        theme.CornersKnob(g, getSliderBounds(band->PostGainSlider.getBounds()) + bandCorner, COMP_PADDING / 3);
        if (!theme.reduced3DBorders) {
            theme.CornersKnob(g, getSliderBounds(band->PeakGainSlider.getBounds()) + bandCorner, COMP_PADDING / 4);
            theme.CornersKnob(g, getSliderBounds(band->PeakFreqSlider.getBounds()) + bandCorner, COMP_PADDING / 3);
            theme.CornersKnob(g, getSliderBounds(band->PeakQSlider.getBounds()) + bandCorner, COMP_PADDING / 4);
            theme.CornersKnob(g, getSliderBounds(band->SmearAmtSlider.getBounds()) + bandCorner, COMP_PADDING / 4);
            theme.CornersKnob(g, getSliderBounds(band->SmearLengthSlider.getBounds()) + bandCorner, COMP_PADDING / 4);
            theme.CornersRect(g, band->BtnMute.getBounds() + bandCorner, COMP_PADDING / 4, COMP_PADDING / 3);
            theme.CornersRect(g, band->BtnSolo.getBounds() + bandCorner, COMP_PADDING / 4, COMP_PADDING / 3);
            theme.CornersRect(g, band->BtnBypass.getBounds() + bandCorner, COMP_PADDING / 4, COMP_PADDING / 3);
        }
    }
#pragma endregion

    // write version info
    String version = "v.";
    version << ProjectInfo::versionString;
    g.setFont(TEXT_NORMAL);
    g.setColour(theme.COLOR_TEXT_SECONDARY);
    Rectangle<int> versionBox = Rectangle<int>(
        COMP_PADDING,
        getHeight() - TEXT_NORMAL - 8,
        g.getCurrentFont().getStringWidth(version),
        TEXT_NORMAL + 8
    );
    g.drawFittedText(version, versionBox, Justification::centred, 1);
}

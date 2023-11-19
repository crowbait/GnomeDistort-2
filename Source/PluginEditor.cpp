/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GnomeDistort2AudioProcessorEditor::GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor& p, const std::map<GnomeDistort2Parameters::TreeParameter, juce::String> pm)
    : AudioProcessorEditor(&p), audioProcessor(p),
    BandControlsLo(Band::Lo, p.apvts, pm, knobOverlay),
    BandControlsMid(Band::Mid, p.apvts, pm, knobOverlay),
    BandControlsHi(Band::Hi, p.apvts, pm, knobOverlay) {

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }

    knobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_128_png, BinaryData::knob_overlay_128_pngSize);

    setSize(960, 720);
    checkForUpdates();
}

GnomeDistort2AudioProcessorEditor::~GnomeDistort2AudioProcessorEditor() {}

//==============================================================================

void GnomeDistort2AudioProcessorEditor::paint(juce::Graphics& g) {

}

void GnomeDistort2AudioProcessorEditor::resized() {
    const int padding = 32; // (windowWidth - (padding * 1.5)) % 4 MUST be 0

    auto bounds = getLocalBounds();
    bounds.removeFromLeft(padding / 2);
    bounds.removeFromRight(padding / 2);
    bounds.removeFromBottom(padding);

    int bandWidth = (bounds.getWidth() - (padding * 1.5f)) * 0.25f; // padding * 1.5 = 3 times half padding -> space between control columns

    auto switchesArea = bounds.removeFromTop(padding);
    bounds.removeFromTop(padding);

    PostBandControl.setBounds(bounds.removeFromRight(bandWidth));
    bounds.removeFromRight(padding * 0.5f);

    auto displayArea = bounds.removeFromTop(bounds.getHeight() / 5);
    displayArea.removeFromLeft(padding / 2);
    displayArea.removeFromRight(padding / 2);
    displayArea.removeFromBottom(padding);
    Display.setBounds(displayArea);
    PreBandControl.setBounds(bounds.removeFromTop(bounds.getHeight() / 5));

    bounds.removeFromTop(padding / 2);
    BandControlsLo.setBounds(bounds.removeFromLeft(bandWidth));
    bounds.removeFromLeft(padding / 2);
    BandControlsMid.setBounds(bounds.removeFromLeft(bandWidth));
    bounds.removeFromLeft(padding / 2);
    BandControlsHi.setBounds(bounds.removeFromLeft(bandWidth));
}

void GnomeDistort2AudioProcessorEditor::checkForUpdates() {
    using namespace juce;
    const URL url = URL("https://api.github.com/repos/crowbait/GnomeDistort-2/releases/latest");
    StringPairArray responseHeaders;
    int statusCode = 0;
    String response;

    File appdir = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory);
    File lastCheckFile = File(appdir.getFullPathName() + File::getSeparatorString() + "CrowbaitVSTs" + File::getSeparatorString() + "GnomeDistort2-update-check");
    if (!lastCheckFile.exists()) lastCheckFile.create();
    StringArray readLines;
    lastCheckFile.readLines(readLines);
    DBG("Last check: " + readLines[0]);

    if (readLines[0] == "" || Time::fromISO8601(readLines[0]).getDayOfMonth() != Time().getCurrentTime().getDayOfMonth()) {
        DBG("Checking for update");
        lastCheckFile.replaceWithText(Time::getCurrentTime().toISO8601(true));
        std::unique_ptr<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
                                                                  750, // timeout in ms
                                                                  &responseHeaders, &statusCode));
        if (stream != nullptr) {
            response = stream->readEntireStreamAsString();
            DBG(response);
            var json = JSON::parse(response);
            String versionString = json.getProperty("tag_name", "Error").toString();
            if (versionString != ProjectInfo::versionString) {
                AlertWindow::showAsync(
                    MessageBoxOptions()
                    .makeOptionsOkCancel(MessageBoxIconType::InfoIcon,
                                         "Update available",
                                         "A new update is available: " + versionString + newLine +
                                         "You have version " + ProjectInfo::versionString + " installed." + newLine,
                                         "Download", "Maybe later"),
                    [](int res) { // res: 1 = Download, 0 = Maybe later
                    if (res == 1) URL("https://github.com/crowbait/GnomeDistort/releases").launchInDefaultBrowser();
                });
            } else DBG("No update available");
            return;
        }
        if (statusCode != 0 && statusCode != 200) {
            DBG("Failed to connect, status " + String(statusCode));
        } else DBG("Failed to connect.");
    } else DBG("Already checked for update today.");
    lastCheckFile.~File();
}

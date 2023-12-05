#include "PluginEditor.h"

//==============================================================================
GnomeDistort2AudioProcessorEditor::GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor& p,
                                                                     juce::AudioProcessorValueTreeState* apvts,
                                                                     const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* pm)
    : AudioProcessorEditor(&p),
    DisplayArea(&p, apvts, pm, &settings),
    PreBandControl(apvts, pm, &knobOverlay, &primaryColor, &secondaryColor),
    BandControlsLo(Band::Lo, apvts, pm, &knobOverlay, &primaryColor, &secondaryColor),
    BandControlsMid(Band::Mid, apvts, pm, &knobOverlay, &primaryColor, &secondaryColor),
    BandControlsHi(Band::Hi, apvts, pm, &knobOverlay, &primaryColor, &secondaryColor),
    PostBandControl(apvts, pm, &knobOverlay, &primaryColor, &secondaryColor),

    PostGainSlider("GAIN", false, &knobOverlay, &primaryColor),
    MixSlider("MIX", false, &knobOverlay, &primaryColor),
    AttachPostGainSlider(*apvts, pm->at(GnomeDistort2Parameters::TreeParameter::PostGainGlobal), PostGainSlider),
    AttachMixSlider(*apvts, pm->at(GnomeDistort2Parameters::TreeParameter::DryWet), MixSlider),

    SwitchDisplayOnButton("ON", "OFF", GnomeDistort2UIConst::TEXT_NORMAL, juce::Colours::white, juce::Colours::grey),
    SwitchDisplayHQButton("HQ", GnomeDistort2UIConst::TEXT_NORMAL, juce::Colours::white, juce::Colours::grey),
    LinkDonateButton("Donate", juce::Colours::lightgrey, GnomeDistort2UIConst::TEXT_NORMAL, true),
    LinkGithubButton("Github", juce::Colours::lightgrey, GnomeDistort2UIConst::TEXT_NORMAL, true) {

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }

    knobOverlay = juce::ImageCache::getFromMemory(BinaryData::knob_overlay_128_png, BinaryData::knob_overlay_128_pngSize);

    auto display = juce::Component::SafePointer<GnomeDistort2Controls::DisplayComponent>(&DisplayArea.displayComp);
    auto displayOnSwitch = juce::Component::SafePointer<GnomeDistort2Controls::TextSwitch>(&SwitchDisplayOnButton);
    auto* settingsPtr = &settings;
    SwitchDisplayOnButton.onClick = [display, displayOnSwitch, settingsPtr]() {
        display->isEnabled = displayOnSwitch->getToggleState();
        settingsPtr->displayEnabled = display->isEnabled;
        settingsPtr->saveSettings();
    };
    auto displayHQSwitch = juce::Component::SafePointer<GnomeDistort2Controls::TextSwitch>(&SwitchDisplayHQButton);
    SwitchDisplayHQButton.onClick = [display, displayHQSwitch, settingsPtr]() {
        display->isHQ = displayHQSwitch->getToggleState();
        settingsPtr->displayHQ = display->isHQ;
        settingsPtr->saveSettings();
    };
    SwitchDisplayOnButton.setToggleState(settings.displayEnabled, false);
    SwitchDisplayHQButton.setToggleState(settings.displayHQ, false);

    LinkGithubButton.onClick = []() {
        juce::URL("https://github.com/crowbait/GnomeDistort").launchInDefaultBrowser();
    };
    LinkDonateButton.onClick = []() {
        juce::URL("https://ko-fi.com/crowbait").launchInDefaultBrowser();
    };

    setSize(960, 720);
    checkForUpdates();
}

GnomeDistort2AudioProcessorEditor::~GnomeDistort2AudioProcessorEditor() {}

//==============================================================================

void GnomeDistort2AudioProcessorEditor::paint(juce::Graphics& g) {
    g.drawImageAt(background, 0, 0);
}

void GnomeDistort2AudioProcessorEditor::resized() {
    const int padding = 32; // (windowWidth - (padding * 1.5)) % 4 MUST be 0

    auto bounds = getLocalBounds();
    bounds.removeFromLeft(padding / 2);
    bounds.removeFromRight(padding / 2);
    bounds.removeFromBottom(padding);

    int bandWidth = (bounds.getWidth() - (padding * 2)) * 0.25f; // padding * 2 = 4 times half padding -> space between control columns

    bounds.removeFromTop(padding);
    auto switchesArea = bounds.removeFromTop(padding);
    switchesArea.removeFromLeft(padding / 2);
    auto displaySwitchesArea = switchesArea.removeFromLeft(padding * 2);
    displaySwitchesArea.removeFromTop(displaySwitchesArea.getHeight() - (padding / 2));
    SwitchDisplayOnButton.setBounds(displaySwitchesArea.removeFromLeft(padding));
    SwitchDisplayHQButton.setBounds(displaySwitchesArea.removeFromLeft(padding));
    LinkDonateButton.setBounds(switchesArea.removeFromRight(padding * 2));
    LinkGithubButton.setBounds(switchesArea.removeFromRight(padding * 2));

    auto postArea = bounds.removeFromRight(bandWidth);

    auto displayArea = bounds.removeFromTop(bounds.getHeight() / 5);
    displayArea.removeFromLeft(padding / 2);
    displayArea.removeFromRight(padding / 2);
    displayArea.removeFromBottom(padding);
    DisplayArea.setBounds(displayArea);
    PreBandControl.setBounds(bounds.removeFromTop(bounds.getHeight() / 5));

    bounds.removeFromTop(padding);
    BandControlsLo.setBounds(bounds.removeFromLeft(bandWidth));
    bounds.removeFromLeft(padding);
    BandControlsMid.setBounds(bounds.removeFromLeft(bandWidth));
    bounds.removeFromLeft(padding);
    BandControlsHi.setBounds(bounds.removeFromLeft(bandWidth));

    postArea.removeFromLeft(padding);
    PostBandControl.setBounds(postArea.removeFromBottom(BandControlsLo.getBounds().getHeight()));
    postArea.removeFromBottom(padding);
    PostGainSlider.setBounds(postArea.removeFromBottom(PreBandControl.getBounds().getHeight()));
    MixSlider.setBounds(postArea.removeFromTop(DisplayArea.getBounds().getHeight()));

    paintBackground();
}

void GnomeDistort2AudioProcessorEditor::checkForUpdates() {
    using namespace juce;
    const URL url = URL("https://api.github.com/repos/crowbait/GnomeDistort-2/releases/latest");
    StringPairArray responseHeaders;
    int statusCode = 0;
    String response;

    DBG("Last check: " + settings.lastUpdateCheck.toISO8601(true));
    Time now = Time().getCurrentTime();
    if (settings.lastUpdateCheck.toMilliseconds() < now.toMilliseconds() - (24 * 60 * 60 * 1000)) {   // lastcheck < (now - 24h)
        DBG("Checking for update");
        settings.lastUpdateCheck = now;
        settings.saveSettings();

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
}

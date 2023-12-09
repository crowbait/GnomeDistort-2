#include "PluginEditor.h"
#include "Helpers/UpdateCheck.h"
#include "UI/Theme/UIConsts.h"
#include "UI/Theme/Theme-GnomeDefault.h"
#include "UI/Theme/Theme-Oscilloscope.h"
#include "UI/Theme/Theme-CleanDarkRed.h"
#include "UI/Theme/Theme-CleanDarkGreen.h"
#include "UI/Theme/Theme-CleanDarkBlue.h"
#include "UI/Windows/AboutWindow.h"

//==============================================================================
GnomeDistort2AudioProcessorEditor::GnomeDistort2AudioProcessorEditor(GnomeDistort2AudioProcessor& p,
                                                                     juce::AudioProcessorValueTreeState* apvts,
                                                                     const std::map<GnomeDistort2Parameters::TreeParameter, juce::String>* pm)
    : AudioProcessorEditor(&p),
    DisplayArea(&p, apvts, pm, &settings, &theme),
    PreBandControl(apvts, pm, &theme),
    BandControlsLo(Band::Lo, apvts, pm, &theme),
    BandControlsMid(Band::Mid, apvts, pm, &theme),
    BandControlsHi(Band::Hi, apvts, pm, &theme),
    PostBandControl(apvts, pm, &theme),

    PostGainSlider("GAIN", false, &theme, GnomeDistort2Controls::SliderLabeledValue::PRIMARY),
    MixSlider("MIX", false, &theme, GnomeDistort2Controls::SliderLabeledValue::PRIMARY),
    AttachPostGainSlider(*apvts, pm->at(GnomeDistort2Parameters::TreeParameter::PostGainGlobal), PostGainSlider),
    AttachMixSlider(*apvts, pm->at(GnomeDistort2Parameters::TreeParameter::DryWet), MixSlider),

    SwitchDisplayOnButton("ON", "OFF", GnomeDistort2Theme::TEXT_NORMAL, &theme),
    SwitchDisplayHQButton("HQ", GnomeDistort2Theme::TEXT_NORMAL, &theme),
    LinkDonateButton("Donate", &theme, GnomeDistort2Theme::TEXT_NORMAL, true),
    LinkGithubButton("Github", &theme, GnomeDistort2Theme::TEXT_NORMAL, true),
    AboutButton("About", juce::DrawableButton::ImageRaw) {

    setThemeFromSettings(false);

    for (auto* comp : getComponents()) {
        addAndMakeVisible(comp);
    }

    auto display = juce::Component::SafePointer<GnomeDistort2Controls::DisplayComponent>(&DisplayArea.displayComp);
    auto displayOnSwitch = juce::Component::SafePointer<GnomeDistort2Controls::TextSwitch>(&SwitchDisplayOnButton);
    auto displayHQSwitch = juce::Component::SafePointer<GnomeDistort2Controls::TextSwitch>(&SwitchDisplayHQButton);
    auto* settingsPtr = &settings;
    auto thisPtr = juce::Component::SafePointer<GnomeDistort2AudioProcessorEditor>(this);

    SwitchDisplayOnButton.onClick = [display, displayOnSwitch, settingsPtr]() {
        display->isEnabled = displayOnSwitch->getToggleState();
        if (display->isEnabled) {
            display->startTimerHz(30);
        } else {
            display->stopTimer();
            juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
            if (mml.lockWasGained()) {
                display->repaint();
            } else DBG("No Lock");
        }
        settingsPtr->displayEnabled = display->isEnabled;
        settingsPtr->saveSettings();
    };
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
    AboutButton.setLookAndFeel(&invisButtonLnF);
    AboutButton.onClick = [thisPtr]() {
        GnomeDistort2Windows::showAboutWindow(thisPtr);
    };

    setSize(960, 720);

    juce::String newVersion = GnomeDistort2Helpers::checkForUpdate(juce::URL("https://api.github.com/repos/crowbait/GnomeDistort-2/releases/latest"),
                                                                   settings.lastUpdateCheck, &settings);
    if (newVersion != "") {
        juce::AlertWindow::showAsync(
            juce::MessageBoxOptions()
            .makeOptionsOkCancel(juce::MessageBoxIconType::InfoIcon,
                                 "GnomeDistort 2: Update available",
                                 "A new update is available: " + newVersion + juce::newLine +
                                 "You have version " + ProjectInfo::versionString + " installed." + juce::newLine,
                                 "Download", "Maybe later"),
            [](int res) { // res: 1 = Download, 0 = Maybe later
            if (res == 1) juce::URL("https://github.com/crowbait/GnomeDistort/releases").launchInDefaultBrowser();
        });
    }
}

GnomeDistort2AudioProcessorEditor::~GnomeDistort2AudioProcessorEditor() {
    AboutButton.setLookAndFeel(nullptr);
}

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
    switchesArea.translate(0, -padding / 4);
    switchesArea.removeFromLeft(padding / 2);
    switchesArea.removeFromRight(bandWidth + (padding / 2));
    SwitchDisplayOnButton.setBounds(switchesArea.removeFromLeft(padding));
    SwitchDisplayHQButton.setBounds(switchesArea.removeFromLeft(padding));
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

    AboutButton.setBounds(juce::Rectangle<int>(getWidth() - 80, getHeight() - 100, 80, 100));

    paintBackground();
}

void GnomeDistort2AudioProcessorEditor::setThemeFromSettings(bool callRedraw) {
    switch (settings.theme) {
        case GnomeDistort2Theme::GnomeDefault: theme = GnomeDistort2Theme::getTheme_GnomeDefault(); break;
        case GnomeDistort2Theme::Oscilloscope: theme = GnomeDistort2Theme::getTheme_Oscilloscope(); break;
        case GnomeDistort2Theme::CleanDarkRed: theme = GnomeDistort2Theme::getTheme_CleanDarkRed(); break;
        case GnomeDistort2Theme::CleanDarkGreen: theme = GnomeDistort2Theme::getTheme_CleanDarkGreen(); break;
        case GnomeDistort2Theme::CleanDarkBlue: theme = GnomeDistort2Theme::getTheme_CleanDarkBlue(); break;
    }
    PreBandControl.applyTheme(&theme);
    BandControlsLo.applyTheme(&theme, callRedraw);
    BandControlsMid.applyTheme(&theme, callRedraw);
    BandControlsHi.applyTheme(&theme, callRedraw);
    PostBandControl.applyTheme(&theme);
    if (callRedraw) {
        paintBackground();
        DisplayArea.applyTheme();

        repaint();
    }
}

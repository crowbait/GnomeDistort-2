#pragma once
#include <JuceHeader.h>
#include "../../PluginEditor.h"
#include "../Controls/ComboBoxLnF.h"
#include "../Theme/Theme.h"
#include "../Theme/UIConsts.h"

namespace GnomeDistort2Windows {
    struct AboutWindow : juce::Component {
        AboutWindow(GnomeDistort2AudioProcessorEditor* editor) :
            theme(&editor->theme),
            selectedTheme(editor->settings.theme),
            lnfCombo(&editor->theme) {

            for (auto* comp : getComponents()) {
                addAndMakeVisible(comp);
            }

            themeSelect.addItemList(GnomeDistort2Theme::ThemeOptions, 1);
            themeSelect.setSelectedId(selectedTheme + 1);
            themeSelect.setLookAndFeel(&lnfCombo);
            auto selectPtr = juce::Component::SafePointer<juce::ComboBox>(&themeSelect);
            themeSelect.onChange = [editor, selectPtr]() {
                editor->settings.theme = (GnomeDistort2Theme::Themes)(selectPtr->getSelectedId() - 1);
                editor->settings.saveSettings();
                editor->setThemeFromSettings(true);
            };

            setOpaque(true);
            setSize(360, 240);
        }
        ~AboutWindow() {
            themeSelect.setLookAndFeel(nullptr);
        }

        void paint(juce::Graphics& g) override {
            using namespace juce;
            g.fillAll(theme->COLOR_BG_AREAS);
            g.drawImageWithin(ImageCache::getFromMemory(BinaryData::logo_small_png, BinaryData::logo_small_pngSize),
                              20, GnomeDistort2Theme::COMP_PADDING, 320, 40,
                              RectanglePlacement::centred);
            auto bounds = getLocalBounds();
            bounds.removeFromTop(GnomeDistort2Theme::COMP_PADDING + 40 + GnomeDistort2Theme::COMP_PADDING);
            bounds.removeFromLeft(GnomeDistort2Theme::COMP_PADDING * 2);
            bounds.removeFromRight(GnomeDistort2Theme::COMP_PADDING * 2);
            bounds.removeFromBottom(GnomeDistort2Theme::COMP_PADDING + GnomeDistort2Theme::SELECT_HEIGHT + GnomeDistort2Theme::COMP_PADDING);
            g.setColour(theme->COLOR_TEXT_PRIMARY);
            g.setFont(GnomeDistort2Theme::TEXT_NORMAL);
            g.drawFittedText("GnomeDistort 2", bounds.removeFromTop(GnomeDistort2Theme::TEXT_NORMAL), juce::Justification::bottomLeft, 1);
            g.drawFittedText("Licensed under GPL-3.0", bounds.removeFromTop(GnomeDistort2Theme::TEXT_NORMAL + (GnomeDistort2Theme::COMP_PADDING / 2)), juce::Justification::bottomLeft, 1);
            g.drawFittedText("Proudly made by Crowbait.", bounds.removeFromTop(GnomeDistort2Theme::TEXT_NORMAL + (GnomeDistort2Theme::COMP_PADDING / 2)), juce::Justification::bottomLeft, 1);
            bounds.removeFromTop(GnomeDistort2Theme::TEXT_NORMAL + (GnomeDistort2Theme::COMP_PADDING / 2));
            g.drawFittedText("Special thanks to Mo & Retron.", bounds.removeFromTop(GnomeDistort2Theme::TEXT_NORMAL + (GnomeDistort2Theme::COMP_PADDING / 2)), juce::Justification::bottomLeft, 1);
        }
        void resized() override {
            auto bounds = getLocalBounds();
            bounds.expand(-GnomeDistort2Theme::COMP_PADDING, -GnomeDistort2Theme::COMP_PADDING);
            themeSelect.setBounds(bounds.removeFromBottom(GnomeDistort2Theme::SELECT_HEIGHT));
        }

    private:
        const GnomeDistort2Theme::Theme* theme;

        GnomeDistort2Theme::Themes selectedTheme;

        juce::ComboBox themeSelect;
        GnomeDistort2Controls::LnFComboBox lnfCombo;

        std::vector<juce::Component*> getComponents() {
            return {
                &themeSelect
            };
        }
    };

    //==============================================================================

    static void showAboutWindow(GnomeDistort2AudioProcessorEditor* editor) {
        auto comp = std::make_unique<AboutWindow>(editor);
        juce::DialogWindow::LaunchOptions launch;
        launch.dialogTitle = "About";
        launch.content.setOwned(comp.release());
        launch.componentToCentreAround = editor;
        launch.useNativeTitleBar = false;
        launch.resizable = false;
        launch.launchAsync();
    }
}

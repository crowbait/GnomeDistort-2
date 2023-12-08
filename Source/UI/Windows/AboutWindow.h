#pragma once
#include <JuceHeader.h>
#include "../../PluginEditor.h"
#include "../Controls/ComboBoxLnF.h"
#include "../Theme/Theme.h"
#include "../Theme/UIConsts.h"

namespace GnomeDistort2Windows {
    struct AboutWindow : juce::Component {
        AboutWindow(GnomeDistort2AudioProcessorEditor* editor) :
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

            setSize(360, 240);
        }
        ~AboutWindow() {
            themeSelect.setLookAndFeel(nullptr);
        }

        void paint(juce::Graphics& g) override {}
        void resized() override {
            auto bounds = getLocalBounds();
            bounds.expand(-GnomeDistort2Theme::COMP_PADDING, -GnomeDistort2Theme::COMP_PADDING);
            themeSelect.setBounds(bounds.removeFromBottom(GnomeDistort2Theme::SELECT_HEIGHT));
        }

    private:
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
        launch.dialogBackgroundColour = editor->theme.COLOR_BG_AREAS;
        launch.content.setOwned(comp.release());
        launch.componentToCentreAround = editor;
        launch.useNativeTitleBar = false;
        launch.resizable = false;
        launch.launchAsync();
    }
}

#pragma once
#include <JuceHeader.h>
#include "../UI/Theme/Theme.h"

namespace GnomeDistort2Helpers {
    // manages application-level settings
    struct Settings {
        Settings() {
            loadSettings();
        }

        juce::Time lastUpdateCheck = juce::Time().getCurrentTime();
        bool displayEnabled = true;
        bool displayHQ = true;
        GnomeDistort2Theme::Themes theme = GnomeDistort2Theme::GnomeDefault;

        void loadSettings() {
            auto map = getLineTexts();
            juce::StringArray lines = {};
            if (file.readLines(&lines)) {
                auto getValue = [&map, &lines](LinePosition pos) {
                    if (lines[pos].length() > 0 &&
                        lines[pos].startsWith(map.at(pos)))
                        return lines[pos].replace(map.at(pos), "");
                    return juce::String("errval");
                };
                if (getValue(PosLastUpdateCheck) != "errval") lastUpdateCheck = juce::Time::fromISO8601(getValue(PosLastUpdateCheck));
                displayEnabled = getValue(PosDisplayEnabled) == "TRUE";
                displayHQ = getValue(PosDisplayHQ) == "TRUE";
                if (getValue(PosTheme) != "errval") theme = (GnomeDistort2Theme::Themes)getValue(PosTheme).getIntValue();
            } else {
                saveSettings();
            }
        }

        void saveSettings() {
            auto map = getLineTexts();
            std::vector<juce::String> lines(map.size());

            auto addValue = [&map, &lines](LinePosition pos, juce::String string) {
                lines[pos] = map.at(pos) + string;
            };
            auto addBoolValue = [&map, &lines](LinePosition pos, bool val) {
                lines[pos] = map.at(pos) + (val ? "TRUE" : "FALSE");
            };
            addValue(PosLastUpdateCheck, lastUpdateCheck.toISO8601(true));
            addBoolValue(PosDisplayEnabled, displayEnabled);
            addBoolValue(PosDisplayHQ, displayHQ);
            addValue(PosTheme, juce::String(theme));

            file.saveLines(&lines);
        }

    private:
        // responsible for managing the persistent settings file
        struct SettingsFile {
            SettingsFile() {
                if (!file.exists()) file.create();
            }

            bool readLines(juce::StringArray* destArray) {
                if (!file.existsAsFile() || file.getSize() < 4) {
                    DBG("No settings file to read");
                    return false;
                }
                file.readLines(*destArray);
                return true;
            }

            bool saveLines(std::vector<juce::String>* lines) {
                if (!file.existsAsFile()) {
                    jassertfalse;
                    return false;
                }
                juce::String out = "";
                for (juce::String line : *lines) {
                    out << line << '\n';
                }
                file.replaceWithText(out);
                return true;
            }

        private:
            juce::File file = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("CrowbaitVSTs").getChildFile("GnomeDistort2-settings");

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsFile)
        }; // end of SettingsFile struct

        enum LinePosition {
            PosLastUpdateCheck,
            PosDisplayEnabled,
            PosDisplayHQ,
            PosTheme
        };
        const juce::String lineDivide = "=";
        const std::map<LinePosition, juce::String> getLineTexts() {
            return {
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //                                                                                                          //
                //      ONLY ADD, NEVER CHANGE STRINGS (won't break, but settings will not be loaded for that variable)     //
                //          -> same for changing settings order instead of adding                                           //
                //                                                                                                          //
                //////////////////////////////////////////////////////////////////////////////////////////////////////////////
                {PosLastUpdateCheck, "LastUpdateCheck" + lineDivide},
                {PosDisplayEnabled, "DisplayOn" + lineDivide},
                {PosDisplayHQ, "DisplayHQ" + lineDivide},
                {PosTheme, "Theme" + lineDivide}
            };
        };

        SettingsFile file;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Settings)
    };
}

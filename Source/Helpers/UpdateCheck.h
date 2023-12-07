#pragma once
#include <JuceHeader.h>
#include "Settings.h"

namespace GnomeDistort2Helpers {
    static juce::String checkForUpdate(const juce::URL url, const juce::Time lastCheck, Settings* settings) {
        using namespace juce;
        StringPairArray responseHeaders;
        int statusCode = 0;
        String response;

        DBG("Last check: " + lastCheck.toISO8601(true));
        const Time now = Time().getCurrentTime();
        if (lastCheck.toMilliseconds() < now.toMilliseconds() - (24 * 60 * 60 * 1000)) {   // lastcheck < (now - 24h)
            DBG("Checking for update");
            settings->lastUpdateCheck = now;
            settings->saveSettings();

            std::unique_ptr<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
                                                                      750, // timeout in ms
                                                                      &responseHeaders, &statusCode));
            if (stream != nullptr) {
                response = stream->readEntireStreamAsString();
                DBG(response);
                var json = JSON::parse(response);
                String versionString = json.getProperty("tag_name", "Error").toString();
                if (versionString != ProjectInfo::versionString) return versionString;
                return "";
            }
            if (statusCode != 0 && statusCode != 200) {
                DBG("Failed to connect, status " + String(statusCode));
            } else DBG("Failed to connect.");
        } else DBG("Already checked for update today.");
        return "";
    }
}

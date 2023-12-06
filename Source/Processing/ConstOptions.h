#pragma once
#include <JuceHeader.h>

namespace GnomeDistort2Parameters {
    namespace Options {
        enum FilterSlope {
            Slope12,
            Slope24,
            Slope36,
            Slope48
        };
        const juce::StringArray FilterSlopeOptions = {
            "12 dB",
            "24 dB",
            "36 dB",
            "48 dB"
        };

        enum WaveshaperFunction {
            HardClip,
            SoftClip,
            Cracked,
            GNOME,
            Warm,
            Quantize,
            Fuzz,
            Hollowing,
            Sin,
            Square,
            Rash,
            Spiked,
            Titruff,
            Lopleft
        };
        const juce::StringArray WaveshaperFunctionOptions = {
            "Hard Clip",
            "Soft Clip",
            "Cracked",
            "GNOME",
            "Warm",
            "Quantize",
            "Fuzz",
            "Hollowing",
            "Sin",
            "Square",
            "Rash",
            "Spiked",
            "Titruff",
            "Lopleft"
        };
    }
}

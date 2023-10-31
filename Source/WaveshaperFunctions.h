#pragma once
#include "ConstOptions.h"

inline static const std::function<float(float)> GetWaveshaperFunction(const WaveshaperFunction& func, const float& amount) {
    switch (func) {
        case HardClip:
            return [amount](float x) { return juce::jlimit(0.f - (1.f - amount), 1.f - amount, x) + (x < 0 ? -amount : amount); };
            break;
        case SoftClip:  // x * sqrt(1+a²) - scaling factor 5
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, x * sqrt(1 + ((amount * 5) * (amount * 5)))); };
            break;
        case Cracked:   // x³ * cos(x*a)³ - scaling factor 9.4
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, (float)(pow(x, 3) * pow((cos(x * amount * 9.4f)), 3))); };
            break;
        case GNOME:   // x - (a/x)
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, x == 0 ? 0 : x - (amount / x)); };
            break;
        case Warm:      // x < 0: x*a   --  x > 0: x*(1+a)
            return [amount](float x) {
                if (x <= 0) return juce::jlimit(-1.f, 1.f, x * (1.f - amount));
                return juce::jlimit(-1.f, 1.f, x * (1.f + amount));
            }; break;
        case Quantize: {
            int numSteps = 1 + std::floor((1 / (amount + 0.01f)) * 2);
            return [numSteps](float x) {
                int quant = (std::min(numSteps, (int)(std::abs(x * numSteps))));
                return juce::jlimit(-1.f, 1.f, (float)(x < 0 ? (0 - ((1.f / numSteps) * quant)) : ((1.f / numSteps) * quant)));
            };
        } break;
        case Fuzz:      // x + (a * sin(10a * x))
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, x + (amount * sin(10 * amount * x))); };
            break;
        case Hollowing: // x * (3a * sin(x)) - x - a
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, x * (3 * amount * sin(x)) - x - amount); };
            break;
        case Sin:
            return [amount](float x) { return juce::jlimit(-1.f, 1.f, 2 * amount * sin(x * 100 * amount) + ((1 - amount) * x)); };
            break;
        case Rash: {      //  -1           -0.8          -0.6          -0.4          -0.2           0            0.2           0.4           0.6           0.8           1
            const float noise[] = { 2.22f, 3.21f, 1.38f, 0.21f, 3.66f, 1.51f, 3.41f, 2.14f, 2.09f, 0.31f, 1.15f, 3.15f, 2.58f, 0.91f, 1.18f, 4.29f, 3.24f, 0.11f, 0.05f, 2.11f, 1.77f };
            return [amount, noise](float x) {
                const float factor =
                    (x < -0.9f) ? noise[0] : (x < -0.8f) ? noise[1] :
                    (x < -0.7f) ? noise[2] : (x < -0.6f) ? noise[3] :
                    (x < -0.5f) ? noise[4] : (x < -0.4f) ? noise[5] :
                    (x < -0.3f) ? noise[6] : (x < -0.2f) ? noise[7] :
                    (x < -0.1f) ? noise[8] : (x < 0.f) ? noise[9] :
                    (x < 0.1f) ? noise[10] : (x < 0.2f) ? noise[11] :
                    (x < 0.3f) ? noise[12] : (x < 0.4f) ? noise[13] :
                    (x < 0.5f) ? noise[14] : (x < 0.6f) ? noise[15] :
                    (x < 0.7f) ? noise[16] : (x < 0.8f) ? noise[17] :
                    (x < 0.9f) ? noise[18] : (x < 1.f) ? noise[19] : noise[20];
                return juce::jlimit(-1.f, 1.f, (factor * x * amount) + (x * (1.f - amount)));
            };
        } break;
        case Spiked: {
            const float pi3p16 = 3 * 3.14159f / 16;
            return [amount, pi3p16](float x) {
                if (x < -pi3p16) return juce::jlimit(-1.f, 1.f, amount + (x * (1.f - amount)));
                if (x < 0) return juce::jlimit(-1.f, 1.f, (sin(8 * x) * amount) + (x * (1.f - amount)));
                if (x < 0.25f) return juce::jlimit(-1.f, 1.f, ((sin(10 * x) + 0.25f) * amount) + (x * (1.f - amount)));
                if (x < 0.5f) return juce::jlimit(-1.f, 1.f, ((sin(10 * x + 1) + 0.25f) * amount) + (x * (1.f - amount)));
                if (x < 0.75f) return juce::jlimit(-1.f, 1.f, ((sin(10 * x + 2) + 0.25f) * amount) + (x * (1.f - amount)));
                return juce::jlimit(-1.f, 1.f, x * (1.f - amount) + amount);
            };
        } break;
        case Titruff: {
            return [amount](float x) {
                if (x > -0.162f && x < 0.162f) return ((-10.f * x * x + 1) * amount) + (x * (1.f - amount));
                return juce::jlimit(-1.f, 1.f, ((-0.5f * x * x + 0.75f) * amount) + (x * (1.f - amount)));
            }; break;
        }
    }
}

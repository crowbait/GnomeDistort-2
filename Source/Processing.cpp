#include "Processing.h"

void GnomeDistort2Processing::Processing::DistBand::updateSettings(const GnomeDistort2Parameters::Parameters::ChainSettings::BandChainSettings& bandChainSettings, double sampleRate) {
    updateCoefficients(
        chain.get<ChainPositions::PeakFilter>().coefficients,
        generatePeakFilter(juce::jmap<float>(bandChainSettings.PeakFreq, LowerFreqBound, UpperFreqBound), bandChainSettings.PeakQ, bandChainSettings.PeakGain, sampleRate)
    );
    chain.get<ChainPositions::PreGain>().setGainDecibels(bandChainSettings.PreGain);
    juce::dsp::Reverb::Parameters reverbParams;
    reverbParams.wetLevel = bandChainSettings.SmearAmount;
    reverbParams.damping = 1.f - bandChainSettings.SmearAmount;
    reverbParams.roomSize = bandChainSettings.SmearLength;
    reverbParams.width = bandChainSettings.SmearLength;
    chain.get<ChainPositions::Reverb>().setParameters(reverbParams);
    chain.get<ChainPositions::Waveshaper>().functionToUse = GetWaveshaperFunction(bandChainSettings.WaveshapeFunction, bandChainSettings.WaveshapeAmount);
    chain.get<ChainPositions::PostGain>().setGainDecibels(bandChainSettings.PostGain);
}

//==============================================================================

void GnomeDistort2Processing::Processing::GnomeDSP::prepare(const juce::dsp::ProcessSpec& spec) {
    preBandChainL.prepare(spec);
    preBandChainR.prepare(spec);

    // LPLo, APLo, HPMidHi, LPMid, HPHi
    LPLo.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    LPLo.prepare(spec);
    APLo.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    APLo.prepare(spec);
    HPMidHi.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    HPMidHi.prepare(spec);
    LPMid.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    LPMid.prepare(spec);
    HPHi.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    HPHi.prepare(spec);
    for (auto& buffer : bandBuffers) buffer.setSize(spec.numChannels, spec.maximumBlockSize);

    juce::dsp::ProcessSpec monoSpec = spec;
    monoSpec.numChannels = 1;
    BandLoL.prepare(monoSpec);
    BandLoR.prepare(monoSpec);
    BandMidL.prepare(monoSpec);
    BandMidR.prepare(monoSpec);
    BandHiL.prepare(monoSpec);
    BandHiR.prepare(monoSpec);
    postBandChainL.prepare(monoSpec);
    postBandChainR.prepare(monoSpec);
    dryWetMixL.prepare(monoSpec);
    dryWetMixR.prepare(monoSpec);
    dryWetMixL.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    dryWetMixR.setMixingRule(juce::dsp::DryWetMixingRule::linear);
}

void GnomeDistort2Processing::Processing::GnomeDSP::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::AudioBlock<float> blockL = block.getSingleChannelBlock(0);
    juce::dsp::AudioBlock<float> blockR = block.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> contextL = (blockL);
    juce::dsp::ProcessContextReplacing<float> contextR = (blockR);
    dryWetMixL.pushDrySamples(blockL);
    dryWetMixR.pushDrySamples(blockR);

    preBandChainL.process(contextL);
    preBandChainR.process(contextR);

    for (auto& fbuffer : bandBuffers) fbuffer = buffer;
    juce::dsp::AudioBlock<float> bandBlockLo(bandBuffers[0]);
    juce::dsp::AudioBlock<float> bandBlockMid(bandBuffers[1]);
    juce::dsp::AudioBlock<float> bandBlockHi(bandBuffers[2]);
    juce::dsp::ProcessContextReplacing<float> cntxLo = juce::dsp::ProcessContextReplacing<float>(bandBlockLo);
    juce::dsp::ProcessContextReplacing<float> cntxMid = juce::dsp::ProcessContextReplacing<float>(bandBlockMid);
    juce::dsp::ProcessContextReplacing<float> cntxHi = juce::dsp::ProcessContextReplacing<float>(bandBlockHi);
    LPLo.process(cntxLo);
    APLo.process(cntxMid);
    HPMidHi.process(cntxMid);
    bandBuffers[2] = bandBuffers[1];    // copy HPMidHi output because it needs to be used two times
    LPMid.process(cntxMid);
    HPHi.process(cntxHi);

    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    buffer.clear();
    auto addFilterBand = [numChannels, numSamples](juce::AudioBuffer<float>& inputBuffer, const juce::AudioBuffer<float>& source) {
        for (int i = 0; i < numChannels; ++i) { // ++i is pre-increment: increments BEFORE first loop run
            inputBuffer.addFrom(i, 0, source, i, 0, numSamples);
        }
    };

    if (!isBypassedLo && (channelsToAdd & 1) == 1) {
        BandLoL.process(juce::dsp::ProcessContextReplacing<float>(bandBlockLo.getSingleChannelBlock(0)));
        BandLoR.process(juce::dsp::ProcessContextReplacing<float>(bandBlockLo.getSingleChannelBlock(1)));
    }
    if (!isBypassedMid && (channelsToAdd & 2) == 2) {
        BandMidL.process(juce::dsp::ProcessContextReplacing<float>(bandBlockMid.getSingleChannelBlock(0)));
        BandMidR.process(juce::dsp::ProcessContextReplacing<float>(bandBlockMid.getSingleChannelBlock(1)));
    }
    if (!isBypassedHi && (channelsToAdd & 4) == 4) {
        BandHiL.process(juce::dsp::ProcessContextReplacing<float>(bandBlockHi.getSingleChannelBlock(0)));
        BandHiR.process(juce::dsp::ProcessContextReplacing<float>(bandBlockHi.getSingleChannelBlock(1)));
    }

    if ((channelsToAdd & 1) == 1) addFilterBand(buffer, bandBuffers[0]);
    if ((channelsToAdd & 2) == 2) addFilterBand(buffer, bandBuffers[1]);
    if ((channelsToAdd & 4) == 4) addFilterBand(buffer, bandBuffers[2]);

    postBandChainL.process(contextL);
    postBandChainR.process(contextR);
    dryWetMixL.mixWetSamples(blockL);
    dryWetMixR.mixWetSamples(blockR);
}

void GnomeDistort2Processing::Processing::GnomeDSP::updateSettings(const GnomeDistort2Parameters::Parameters::ChainSettings& chainSettings, double sampleRate) {
    auto preBandsLoCut = generateLoCutFilter(chainSettings.LoCutFreq, chainSettings.LoCutSlope, sampleRate);
    auto preBandsHiCut = generateHiCutFilter(chainSettings.HiCutFreq, chainSettings.HiCutSlope, sampleRate);
    updateCutFilter(preBandChainL.get<0>(), preBandsLoCut, chainSettings.LoCutSlope);
    updateCutFilter(preBandChainL.get<1>(), preBandsHiCut, chainSettings.HiCutSlope);
    updateCutFilter(preBandChainR.get<0>(), preBandsLoCut, chainSettings.LoCutSlope);
    updateCutFilter(preBandChainR.get<1>(), preBandsHiCut, chainSettings.HiCutSlope);

    LPLo.setCutoffFrequency(chainSettings.BandFreqLoMid);
    APLo.setCutoffFrequency(chainSettings.BandFreqLoMid);
    HPMidHi.setCutoffFrequency(chainSettings.BandFreqLoMid);
    LPMid.setCutoffFrequency(chainSettings.BandFreqMidHi);
    HPHi.setCutoffFrequency(chainSettings.BandFreqMidHi);

    //                    8421
    //                    s321
    channelsToAdd = 0b00000111;   // any solos, bands 3 (Hi), 2 (Mid), 1 (Lo)
    auto setFlags = [](char flags, const bool bandSolo, const bool bandMute, const char bandFlagValue) {
        if (bandSolo) {
            if ((flags & 8) == 8) {             // if any solos have been set
                flags |= bandFlagValue;         // add channel to bitmask
            } else {                            // no solos present
                flags = 0 | 8 | bandFlagValue;  // set all other to off and set solos flag
            }
        }
        if (bandMute) flags &= ~bandFlagValue;  // on mute, AND with bitwise NOT of position to remove band from flags
        return flags;                           // if not mute or solo, return initial value: channelsToAdd is set to "just add all bands" by default
    };
    channelsToAdd = setFlags(channelsToAdd, chainSettings.SoloLo, chainSettings.MuteLo, 1);
    channelsToAdd = setFlags(channelsToAdd, chainSettings.SoloMid, chainSettings.MuteMid, 2);
    channelsToAdd = setFlags(channelsToAdd, chainSettings.SoloHi, chainSettings.MuteHi, 4);

    isBypassedLo = chainSettings.BypassLo;
    if (!isBypassedLo) {
        BandLoL.updateSettings(chainSettings.LoBandSettings, sampleRate);
        BandLoR.updateSettings(chainSettings.LoBandSettings, sampleRate);
        BandLoL.LowerFreqBound = chainSettings.LoCutFreq;
        BandLoR.LowerFreqBound = chainSettings.LoCutFreq;
        BandLoL.UpperFreqBound = chainSettings.BandFreqLoMid;
        BandLoR.UpperFreqBound = chainSettings.BandFreqLoMid;
    }
    isBypassedMid = chainSettings.BypassMid;
    if (!isBypassedMid) {
        BandMidL.updateSettings(chainSettings.MidBandSettings, sampleRate);
        BandMidR.updateSettings(chainSettings.MidBandSettings, sampleRate);
        BandMidL.LowerFreqBound = chainSettings.BandFreqLoMid;
        BandMidR.LowerFreqBound = chainSettings.BandFreqLoMid;
        BandMidL.UpperFreqBound = chainSettings.BandFreqMidHi;
        BandMidR.UpperFreqBound = chainSettings.BandFreqMidHi;
    }
    isBypassedHi = chainSettings.BypassHi;
    if (!isBypassedHi) {
        BandHiL.updateSettings(chainSettings.HiBandSettings, sampleRate);
        BandHiR.updateSettings(chainSettings.HiBandSettings, sampleRate);
        BandHiL.LowerFreqBound = chainSettings.BandFreqMidHi;
        BandHiR.LowerFreqBound = chainSettings.BandFreqMidHi;
        BandHiL.UpperFreqBound = chainSettings.HiCutFreq;
        BandHiR.UpperFreqBound = chainSettings.HiCutFreq;
    }

    postBandChainL.get<0>().functionToUse = GetWaveshaperFunction(chainSettings.WaveshapeFunctionGlobal, chainSettings.WaveshapeAmountGlobal);
    postBandChainR.get<0>().functionToUse = GetWaveshaperFunction(chainSettings.WaveshapeFunctionGlobal, chainSettings.WaveshapeAmountGlobal);
    postBandChainL.get<1>().setGainDecibels(chainSettings.PostGainGlobal);
    postBandChainR.get<1>().setGainDecibels(chainSettings.PostGainGlobal);
    dryWetMixL.setWetMixProportion(chainSettings.DryWet);
    dryWetMixR.setWetMixProportion(chainSettings.DryWet);
}

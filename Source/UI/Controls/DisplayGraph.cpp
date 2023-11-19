#include "DisplayGraph.h"
#include "../UIConsts.h"

GnomeDistort2Controls::DisplayGraph::DisplayGraph(juce::RangedAudioParameter* func, // can't get AudioParameterChoice directly; need to map range to function in parameterValueChanged
                                                  juce::RangedAudioParameter* amt,
                                                  int funcIndex, int amtIndex) :
    funcParamIndex(funcIndex), amtParamIndex(amtIndex),
    funcParam(func), amtParam(amt) {

    funcParam->addListener(this);
    amtParam->addListener(this);
}
GnomeDistort2Controls::DisplayGraph::~DisplayGraph() {
    funcParam->removeListener(this);
    amtParam->removeListener(this);
}

void GnomeDistort2Controls::DisplayGraph::paint(juce::Graphics& g) {
    g.setColour(GnomeDistort2UIConst::COLOR_BG_DARK);
    g.fillRect(getLocalBounds());
}
void GnomeDistort2Controls::DisplayGraph::resized() {

}

void GnomeDistort2Controls::DisplayGraph::parameterValueChanged(int parameterIndex, float newValue) {
    DBG(newValue);
}

juce::Rectangle<int> GnomeDistort2Controls::DisplayGraph::getRenderArea() {
    return juce::Rectangle<int>();
}

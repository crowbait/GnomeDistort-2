#include "DisplayGraph.h"
#include "../UIConsts.h"

DisplayGraph::DisplayGraph(juce::AudioParameterChoice* func, int funcIndex,
                           juce::RangedAudioParameter* amt, int amtIndex) :
    funcParamIndex(funcIndex), amtParamIndex(amtIndex),
    funcParam(func), amtParam(amt) {

    funcParam->addListener(this);
    amtParam->addListener(this);
}
DisplayGraph::~DisplayGraph() {
    funcParam->removeListener(this);
    amtParam->removeListener(this);
}

void DisplayGraph::paint(juce::Graphics& g) {
    g.setColour(GnomeDistort2UIConst::COLOR_BG_DARK);
    g.fillRect(getLocalBounds());
}
void DisplayGraph::resized() {

}

void DisplayGraph::parameterValueChanged(int parameterIndex, float newValue) {

}

juce::Rectangle<int> DisplayGraph::getRenderArea() {

}

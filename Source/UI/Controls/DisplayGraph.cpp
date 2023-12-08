#include "DisplayGraph.h"

GnomeDistort2Controls::DisplayGraph::DisplayGraph(juce::RangedAudioParameter* func, // can't get AudioParameterChoice directly; need to map range to function in parameterValueChanged
                                                  juce::RangedAudioParameter* amt,
                                                  const int funcIndex, const int amtIndex,
                                                  const GnomeDistort2Theme::Theme* theme) :
    funcParamIndex(funcIndex), amtParamIndex(amtIndex),
    funcParam(func), amtParam(amt),
    theme(theme) {

    setOpaque(true);

    funcParam->addListener(this);
    amtParam->addListener(this);
}
GnomeDistort2Controls::DisplayGraph::~DisplayGraph() {
    funcParam->removeListener(this);
    amtParam->removeListener(this);
}

void GnomeDistort2Controls::DisplayGraph::paint(juce::Graphics& g) {
    using namespace juce;
    g.drawImage(background, getLocalBounds().toFloat());

    auto renderArea = getRenderArea();
    int left = renderArea.getX();
    int right = renderArea.getRight();
    int top = renderArea.getY();
    int bottom = renderArea.getBottom();

    auto mapX = [left, right](int x) {return jmap((float)x, (float)left, (float)right, -1.f, 1.f); };
    auto mapY = [top, bottom](float y) {return jmap(y, -1.f, 1.f, (float)bottom, (float)top); };

    g.setColour(theme->COLOR_PRIMARY);
    Path graph;
    graph.startNewSubPath(renderArea.getX(), mapY(waveshaperFunction(-1)));
    for (int x = left + 1; x < right; x++) {
        graph.lineTo(x, mapY(waveshaperFunction(mapX(x))));
    }
    g.strokePath(graph, PathStrokeType(2));   // draw path
}
void GnomeDistort2Controls::DisplayGraph::resized() {
    using namespace juce;
    auto renderArea = getRenderArea();

    background = Image(Image::PixelFormat::RGB, getWidth(), getHeight(), true);
    Graphics g(background);
    g.setColour(theme->COLOR_GRAPH_BG);
    g.fillAll();
    g.setColour(theme->COLOR_GRAPH_MARKINGS);
    g.drawRoundedRectangle(renderArea.toFloat(), 2.f, 1.f);
    g.drawHorizontalLine(renderArea.getY() + (renderArea.getHeight() / 2), renderArea.getX(), renderArea.getRight());
    g.drawVerticalLine(renderArea.getX() + (renderArea.getWidth() / 2), renderArea.getY(), renderArea.getBottom());
}

void GnomeDistort2Controls::DisplayGraph::parameterValueChanged(int parameterIndex, float newValue) {
    if (parameterIndex == funcParamIndex) func = static_cast<GnomeDistort2Parameters::Options::WaveshaperFunction>(juce::jmap(newValue, 0.f, (float)GnomeDistort2Parameters::Options::WaveshaperFunctionOptions.size() - 1));
    if (parameterIndex == amtParamIndex) amount = newValue;
    waveshaperFunction = GnomeDistort2Processing::GetWaveshaperFunction(func, amount);
    juce::MessageManagerLock mml(juce::Thread::getCurrentThread());
    if (mml.lockWasGained()) {
        repaint();
    } else DBG("No Lock");
}

juce::Rectangle<int> GnomeDistort2Controls::DisplayGraph::getRenderArea() {
    auto bounds = getLocalBounds();
    const int remove = 4;
    bounds.removeFromLeft(remove);
    bounds.removeFromTop(remove);
    bounds.removeFromRight(remove);
    bounds.removeFromBottom(remove);
    return bounds;
}

#include "radiallayer.h"

RadialLayer::RadialLayer(Network *network) :
  network_(network)
{

}

RadialLayer::~RadialLayer()
{
  reset();
}

void RadialLayer::setNetwork(Network *network)
{
  network_ = network;
}

void RadialLayer::calculate()
{
  Layer *current = new Layer;
  current->append(network_->getBarById(0));
  next(current);
}

void RadialLayer::reset()
{
  foreach(Layer *layer, layers) {
    delete layer;
  }
  layers.clear();
}

int32_t RadialLayer::height()
{
  int32_t max = -1;

  for(int i=0; i<layers.size(); i++) {
    if(layers[i]->size() > max) {
      max = layers[i]->size();
    }
  }

  return max;
}

int32_t RadialLayer::width()
{
  return layers.size();
}

void RadialLayer::next(RadialLayer::Layer *current)
{
  layers.append(current);
  Layer *nextLayer = new Layer;

  foreach(Bar *bar, *current) {
    foreach(Line *line, bar->lines) {
      if(line->pNoI() == bar) {
        if(line->pNoF() != NULL)
          nextLayer->append(line->pNoF());
      }
    }
  }

  if(!nextLayer->isEmpty()) {
    next(nextLayer);
  }
}


#include "pnlayer.h"

PnLayer::PnLayer(PnNetwork *pnNetwork) :
  pnNetwork_(pnNetwork)
{
}

PnLayer::~PnLayer()
{
  reset();
}

void PnLayer::calculate()
{
  Layer *current = new Layer;
  current->append(pnNetwork_->getBarById(0));
  next(current);
}

void PnLayer::reset()
{
  foreach(Layer *layer, layers) {
    delete layer;
  }

  layers.clear();
}

int32_t PnLayer::height()
{
  int32_t max = -1;

  for(int i=0; i<layers.size(); i++) {
    if(layers[i]->size()>max) {
      max = layers[i]->size();
    }
  }

  return max;
}

int32_t PnLayer::width()
{
  return layers.size();
}

void PnLayer::next(PnLayer::Layer *current)
{
  layers.append(current);
  Layer *nextLayer = new Layer;

  foreach(PnBar *bar, *current) {
    foreach(PnLine *line, bar->lines) {
      if(line->noI() == bar) {
        if(line->noF() != NULL)
          nextLayer->append(line->noF());
      } else {
        if(line->noI() != NULL)
          nextLayer->append(line->noI());
      }
    }
  }

  if(!nextLayer->isEmpty()) {
    next(nextLayer);
  }

}

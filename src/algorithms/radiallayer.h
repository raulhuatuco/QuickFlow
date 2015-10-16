#ifndef ALGORITHMS_RADIALLAYER_H
#define ALGORITHMS_RADIALLAYER_H

#include "models/network.h"

class RadialLayer
{
public:
  typedef QVector<Bar *> Layer;
  
  QVector<Layer *> layers;

  RadialLayer(Network *network);
  ~RadialLayer();
  
  void calculate();
  void reset();
  int32_t height();
  int32_t width();

private:
  Network *network_;
  
  void next(Layer *current);
};

#endif // ALGORITHMS_RADIALLAYER_H

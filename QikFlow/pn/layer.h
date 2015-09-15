#ifndef PNLAYER_H
#define PNLAYER_H

#include <QVector>

#include "pn/bar.h"
#include "pn/network.h"

class PnLayer
{
public:

  typedef QVector<PnBar *> Layer;

  QVector<Layer *> layers;

  PnLayer(PnNetwork *pnNetwork);
  ~PnLayer();

  void calculate();
  void reset();
  int32_t height();
  int32_t width();

private:
  PnNetwork *pnNetwork_;

  void next(Layer *current);
};

#endif // PNLAYER_H

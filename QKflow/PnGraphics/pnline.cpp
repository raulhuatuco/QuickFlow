#include "pnline.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

PnLine::PnLine()
{
  setFlag(ItemIsSelectable);
}

PnLine::~PnLine()
{

}

void PnLine::setNodes(PnBar *noI, PnBar *noF)
{
  noI_ = noI;
  noF_ = noF;

  update();
}


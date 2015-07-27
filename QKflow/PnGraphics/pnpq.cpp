#include "pnpq.h"

#include <QPainter>

QPixmap *PnPq::ico;
QPixmap *PnPq::icoSelected;

PnPq::PnPq()
{

}

PnPq::~PnPq()
{

}

QByteArray PnPq::generateKflowData()
{
  QByteArray KflowData = "slack";

  return KflowData;
}

QRectF PnPq::boundingRect() const
{
  return QRectF(0, 0, 30, 30);
}

void PnPq::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(0, 0, 30, 30, *icoSelected);
  else
          painter->drawPixmap(0, 0, 30, 30, *ico);



}

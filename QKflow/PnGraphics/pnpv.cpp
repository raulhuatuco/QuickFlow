#include "pnpv.h"

#include <QPainter>

QPixmap *PnPv::ico;
QPixmap *PnPv::icoSelected;

PnPv::PnPv()
{

}

PnPv::~PnPv()
{

}

QByteArray PnPv::generateKflowData()
{
  QByteArray KflowData = "slack";

  return KflowData;
}

QRectF PnPv::boundingRect() const
{
  return QRectF(0, 0, 30, 30);
}

void PnPv::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(0, 0, 30, 30, *icoSelected);
  else
          painter->drawPixmap(0, 0, 30, 30, *ico);
}


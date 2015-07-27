#include "pnslack.h"

#include <QPainter>

QPixmap *PnSlack::ico;
QPixmap *PnSlack::icoSelected;

PnSlack::PnSlack()
{

}

PnSlack::~PnSlack()
{

}

QByteArray PnSlack::generateKflowData()
{
  QByteArray KflowData = "slack";

  return KflowData;
}

QRectF PnSlack::boundingRect() const
{
  return QRectF(0, 0, 30, 30);
}

void PnSlack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(0, 0, 30, 30, *icoSelected);
  else
          painter->drawPixmap(0, 0, 30, 30, *ico);
}


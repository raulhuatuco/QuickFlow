#include "pnpq.h"

#include <QPainter>

QPixmap *PnPq::ico;
QPixmap *PnPq::icoSelected;

PnPq::PnPq(uint32_t id)
    : PnBar(id)
{

}

PnPq::~PnPq()
{

}

QString PnPq::barType() {
   return QString("pq");
}

QRectF PnPq::boundingRect() const
{
  return QRectF(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize);
}

void PnPq::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *icoSelected);
  else
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *ico);



}

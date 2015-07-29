#include "pnpv.h"

#include <QPainter>

QPixmap *PnPv::ico;
QPixmap *PnPv::icoSelected;

PnPv::PnPv(uint32_t id)
    : PnBar(id)
{

}

PnPv::~PnPv()
{

}


QString PnPv::barType() {
   return QString("pv");
}

QRectF PnPv::boundingRect() const
{
  return QRectF(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize);
}

void PnPv::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *icoSelected);
  else
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *ico);
}


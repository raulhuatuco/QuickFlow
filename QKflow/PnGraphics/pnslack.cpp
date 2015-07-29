#include "pnslack.h"

#include <QPainter>

QPixmap *PnSlack::ico;
QPixmap *PnSlack::icoSelected;

PnSlack::PnSlack()
    : PnBar(0)
{

}

PnSlack::~PnSlack()
{

}

QString PnSlack::barType() {
  return QString("slack");
}

QRectF PnSlack::boundingRect() const
{
  return QRectF(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize);
}

void PnSlack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *icoSelected);
  else
          painter->drawPixmap(-kIconSize/2, -kIconSize/2, kIconSize, kIconSize, *ico);
}


#include "pnpv.h"

#include <QPainter>

PnPv::PnPv(uint32_t id)
  : PnBar(id) {

}

PnPv::~PnPv() {

}


QString PnPv::barType() {
  return QString("pv");
}

QRectF PnPv::boundingRect() const {
  return QRectF(-kIconSize/2, -kIconSize/2, kIconSize,
                kIconSize);
}

void PnPv::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::gray);

  if (isSelected()) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(-kIconSize/2, -kIconSize/2, kIconSize,
                         kIconSize);
  } else {
    painter->setBrush(Qt::blue);
    painter->drawEllipse(-kIconSize/2, -kIconSize/2, kIconSize,
                         kIconSize);
  }
}


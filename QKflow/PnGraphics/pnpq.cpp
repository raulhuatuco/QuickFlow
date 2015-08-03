#include "pnpq.h"

#include <QPainter>

PnPq::PnPq()
  : PnBar(kInvalidId) {

}

PnPq::PnPq(uint32_t id)
  : PnBar(id) {

}

PnPq::~PnPq() {

}

QString PnPq::barType() {
  return QString("pq");
}

QRectF PnPq::boundingRect() const {
  return QRectF(-kIconSize/2, -kIconSize/2, kIconSize,
                kIconSize);
}

void PnPq::paint(QPainter *painter,
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
    painter->setBrush(Qt::magenta);
    painter->drawEllipse(-kIconSize/2, -kIconSize/2, kIconSize,
                         kIconSize);
  }
}

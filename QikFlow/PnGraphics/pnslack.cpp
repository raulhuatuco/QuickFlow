#include "pnslack.h"

#include <QPainter>

PnSlack::PnSlack() : PnBar(0) {}

PnSlack::~PnSlack() {}

void PnSlack::setMaxGeneration(double maxGeneration) {
  maxGeneration_ = maxGeneration;
}

double PnSlack::maxGeneration() {
  return maxGeneration_;
}

QString PnSlack::barType() {
  return QString("Slack");
}

QRectF PnSlack::boundingRect() const {
  return QRectF(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
}

void PnSlack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::gray);

  if (isSelected()) {
    painter->setBrush(Qt::red);
    painter->drawRect(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  } else {
    painter->setBrush(Qt::green);
    painter->drawRect(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  }
}

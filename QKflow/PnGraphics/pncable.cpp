#include "pncable.h"

#include <QLineF>
#include <QRectF>
#include <QPainter>

PnCable::PnCable() : PnLine(NULL, NULL) {}

PnCable::PnCable(PnBar *noI, PnBar *noF) : PnLine(noI, noF) {
  qreal pi = 3.141592653589793238463;
  QLineF line(noI->pos(), noF->pos());

  qreal radAngle = line.angle() * pi / 180;
  qreal dx = kCableWidth * sin(radAngle);
  qreal dy = kCableWidth * cos(radAngle);
  QPointF offset1 = QPointF(dx, dy);
  QPointF offset2 = QPointF(-dx, -dy);
  selectionArea << line.p1() + offset1 << line.p1() + offset2
                << line.p2() + offset2 << line.p2() + offset1;
  update();
}

PnCable::~PnCable() {}

QString PnCable::lineType() {
  QString type = "Cable";
  return type;
}

QRectF PnCable::boundingRect() const {
  return selectionArea.boundingRect();
}

QPainterPath PnCable::shape() const {
  QPainterPath path;
  path.addPolygon(selectionArea);
  return path;
}

void PnCable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
    painter->setPen(QPen(Qt::red, kCableWidth, Qt::SolidLine));
  else
    painter->setPen(QPen(Qt::black, kCableWidth, Qt::SolidLine));

  painter->drawLine(noI_->pos(), noF_->pos());
}

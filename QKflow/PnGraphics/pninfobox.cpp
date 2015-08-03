#include "pninfobox.h"

#include <QPainter>

PnInfoBox::PnInfoBox(qreal px, qreal py, QString txt)
  : txt_(txt) {
  setPos(px, py);
  setZValue(2);
}

PnInfoBox::~PnInfoBox() {

}

void PnInfoBox::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
  painter->setBrush(QColor::fromRgbF(1.0,1.0,1.0));

  QRectF main_rect = QRectF(-kBoxWidth/2, -kBoxHeight -10,
                            kBoxWidth, kBoxHeight);

  painter->drawRoundRect(main_rect, 10, 10);

  painter->setBrush(Qt::black);
  QPolygonF pol_base;
  pol_base << QPointF(-10.0, -10.0) << QPointF(0.0,
           0.0) << QPointF(10.0,-10.0);
  painter->drawPolygon(pol_base);
  painter->drawText(-kBoxWidth/2 + kMargin,
                    -kBoxHeight + kMargin,
                    kBoxWidth - kMargin, kBoxHeight - kMargin, Qt::AlignLeft,
                    txt_);



}

QRectF PnInfoBox::boundingRect() const {

  return QRectF(-kBoxWidth/2, -kBoxHeight -10, kBoxWidth,
                kBoxHeight + 10);

}


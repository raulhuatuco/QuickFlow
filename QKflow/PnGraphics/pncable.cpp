#include "pncable.h"

#include <QLineF>
#include <QRectF>
#include <QPainter>


PnCable::PnCable()
  : cableColor_(Qt::white),cablePen_(cableColor_, 6,Qt::SolidLine)
{

}

PnCable::~PnCable()
{

}

void PnCable::setNodes(PnBar *noI, PnBar *noF)
{
  QRectF noIRect = noI->boundingRect ();
  QRectF noFRect = noF->boundingRect ();

  cablePos_ = QLineF(mapFromItem(noI, noIRect.width ()/2, noIRect.height ()/2),
                     mapFromItem(noF, noFRect.width ()/2, noFRect.height ()/2));
  PnLine::setNodes (noI, noF);

}

QByteArray PnCable::generateKflowData()
{
  QByteArray KFlowData;
  return KFlowData;
}

QRectF PnCable::boundingRect() const
{
  qreal extra = (cablePen_.width() + 50) / 2.0;

  return QRectF(cablePos_.p1(), QSizeF(cablePos_.p2().x() - cablePos_.p1().x(),
                                    cablePos_.p2().y() - cablePos_.p1().y()))
      .normalized()
      .adjusted(-extra, -extra, extra, extra);
}

void PnCable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if(isSelected ())
  painter->setPen(QPen(Qt::red, 6, Qt::SolidLine));
  else
  painter->setPen(cablePen_);

  painter->drawLine(cablePos_);
}
QLineF PnCable::cablePos() const
{
  return cablePos_;
}

void PnCable::setCablePos(const QLineF &cablePos)
{
  cablePos_ = cablePos;
}

QPen PnCable::cablePen() const
{
  return cablePen_;
}

void PnCable::setCablePen(const QPen &cablePen)
{
  cablePen_ = cablePen;
}

QColor PnCable::cableColor() const
{
  return cableColor_;
}

void PnCable::setCableColor(const QColor &cableColor)
{
  cableColor_ = cableColor;
}



#include "pnline.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

PnLine::PnLine() :
  Zaa(0,0),
  Zab(0,0),
  Zac(0,0),
  Zan(0,0),
  Zbb(0,0),
  Zbc(0,0),
  Zbn(0,0),
  Zcc(0,0),
  Zcn(0,0),
  Znn(0,0),
  Ia(0,0),
  Ib(0,0),
  Ic(0,0),
  In(0,0),
  noI_(NULL),
  noF_(NULL) {
  setFlag(ItemIsSelectable);
  setZValue(0.0);
}

PnLine::~PnLine() {}

PnBar *PnLine::noI() {
  return noI_;
}

PnBar *PnLine::noF() {
  return noF_;
}

void PnLine::setNodes(PnBar *noI, PnBar *noF) {
  QLineF line(noI->pos(), noF->pos());

  qreal radAngle = line.angle() * M_PI / 180.0;
  qreal dx = kCableWidth * sin(radAngle);
  qreal dy = kCableWidth * cos(radAngle);
  QPointF offset1 = QPointF(dx, dy);
  QPointF offset2 = QPointF(-dx, -dy);
  selectionArea << line.p1() + offset1 << line.p1() + offset2
                << line.p2() + offset2 << line.p2() + offset1;

  PnLine::setNodes(noI, noF);
  update();

  noI_ = noI;
  noF_ = noF;

  if ((noI != NULL) && (noF != NULL)) {
    noI->addLine(this);
    noF->addLine(this);
  }
}

std::complex<double> PnLine::Yaa() {
  return std::complex<double>(1.0, 0.0) / Zaa;
}

std::complex<double> PnLine::Yab() {
  return std::complex<double>(1.0, 0.0) / Zab;
}

std::complex<double> PnLine::Yac() {
  return std::complex<double>(1.0, 0.0) / Zac;
}

std::complex<double> PnLine::Yan() {
  return std::complex<double>(1.0, 0.0) / Zan;
}

std::complex<double> PnLine::Ybb() {
  return std::complex<double>(1.0, 0.0) / Zbb;
}

std::complex<double> PnLine::Ybc() {
  return std::complex<double>(1.0, 0.0) / Zbc;
}

std::complex<double> PnLine::Ybn() {
  return std::complex<double>(1.0, 0.0) / Zbn;
}

std::complex<double> PnLine::Ycc() {
  return std::complex<double>(1.0, 0.0) / Zcc;
}

std::complex<double> PnLine::Ycn() {
  return std::complex<double>(1.0, 0.0) / Zcn;
}

std::complex<double> PnLine::Ynn() {
  return std::complex<double>(1.0, 0.0) / Znn;
}

std::complex<double> PnLine::La() {
  return Ia * conj(Ia) * Zaa;
}

std::complex<double> PnLine::Lb() {
  return Ib * conj(Ib) * Zbb;
}

std::complex<double> PnLine::Lc() {
  return Ic * conj(Ic) * Zcc;
}

std::complex<double> PnLine::Ln() {
  return In * conj(In) * Znn;
}

QVariant PnLine::itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value) {
//  if (change == ItemSelectedChange) {
//    if (value == true) {
//      showInfo = true;
//      setZValue(2);
//      update(sceneBoundingRect());
//    } else {
//      showInfo = false;
//      setZValue(1);
//      update(sceneBoundingRect());
//    }
//  }

  return QGraphicsObject::itemChange(change, value);
}

QRectF PnLine::boundingRect() const {
  return selectionArea.boundingRect();
}

QPainterPath PnLine::shape() const {
  QPainterPath path;
  path.addPolygon(selectionArea);
  return path;
}

void PnLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
    painter->setPen(QPen(Qt::red, kCableWidth, Qt::SolidLine));
  else
    painter->setPen(QPen(Qt::black, kCableWidth, Qt::SolidLine));

  painter->drawLine(noI_->pos(), noF_->pos());
}

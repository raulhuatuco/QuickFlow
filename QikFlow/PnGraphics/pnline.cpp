#include "pnline.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

PnLine::PnLine(PnBar *noI, PnBar *noF) : noI_(noI), noF_(noF), infobox_(NULL) {
  setFlag(ItemIsSelectable);
  setZValue(0.0);

  if ((noI != NULL) && (noF != NULL)) {
    noI->addLine(this);
    noF->addLine(this);
  }
}

PnLine::~PnLine() {}

void PnLine::setId(unsigned int id) {
  id_ = id;
}

PnBar *PnLine::NoI() {
  return noI_;
}

PnBar *PnLine::NoF() {
  return noF_;
}

unsigned int PnLine::Id() {
  return id_;
}

void PnLine::setNodes(PnBar *noI, PnBar *noF) {
  noI_ = noI;
  noF_ = noF;

  if ((noI != NULL) && (noF != NULL)) {
    noI->addLine(this);
    noF->addLine(this);
  }
}

void PnLine::setImpedance(std::complex<double> z) {
  z_ = z;
}

std::complex<double> PnLine::Impedance() {
  return z_;
}

void PnLine::setAdmittance(std::complex<double> y) {
  z_ = (std::complex<double>(1.0, 0.0) / y);
}

std::complex<double> PnLine::Admittance() {
  return (std::complex<double>(1.0, 0.0) / z_);
}

void PnLine::setCurrent(std::complex<double> i) {
  i_ = i;
}

std::complex<double> PnLine::Current() {
  return i_;
}

std::complex<double> PnLine::Load() {
  return i_ * conj(i_) * z_;
}

void PnLine::setMaxLoad(double maxLoad) {
  maxLoad_ = maxLoad;
}

double PnLine::MaxLoad() {
  return maxLoad_;
}

QVariant PnLine::itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value) {
  if (change == ItemSelectedChange) {
    if (value == true) {
      qreal px = (noI_->pos().x() + noF_->pos().x()) / 2;
      qreal py = (noI_->pos().y() + noF_->pos().y()) / 2;

      QString txt = "Line : ";

      infobox_ = new PnInfoBox(px, py, txt);
      scene()->addItem(infobox_);

    } else {
      scene()->removeItem(infobox_);
      infobox_ = NULL;
    }
  }

  return QGraphicsObject::itemChange(change, value);
}

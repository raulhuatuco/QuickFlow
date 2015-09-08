#include "pnbar.h"

#include <QGraphicsScene>
#include <QPainter>

PnBar::PnBar()
  : QGraphicsObject(),
    id(kInvalidId),
    Va(0, 0),
    Vb(0, 0),
    Vc(0, 0),
    Sga(0, 0),
    Sgb(0, 0),
    Sgc(0, 0),
    Sla(0, 0),
    Slb(0, 0),
    Slc(0, 0),
    rVa(0, 0),
    rVb(0, 0),
    rVc(0, 0),
    rSga(0, 0),
    rSgb(0, 0),
    rSgc(0, 0),
    rSla(0, 0),
    rSlb(0, 0),
    rSlc(0, 0) {
  setFlag(ItemIsSelectable);
  setZValue(1);
}

PnBar::~PnBar() {
  removeLines();
}

complex<double> PnBar:: Ia() {
  complex<double> I;
  I = conj(Sga - Sla) / conj(Va);
  return I;
}

complex<double> PnBar:: Ib() {
  complex<double> I;
  I = conj(Sgb - Slb) / conj(Vb);
  return I;
}

complex<double> PnBar:: Ic() {
  complex<double> I;
  I = conj(Sgc - Slc) / conj(Vc);
  return I;
}

complex<double> PnBar:: rIa() {
  complex<double> I;
  I = conj(rSga - rSla) / conj(rVa);
  return I;
}

complex<double> PnBar:: rIb() {
  complex<double> I;
  I = conj(rSgb - rSlb) / conj(rVb);
  return I;
}

complex<double> PnBar:: rIc() {
  complex<double> I;
  I = conj(rSgc - rSlc) / conj(rVc);
  return I;
}

void PnBar::addLine(PnLine *line) {
  lines.append(line);
}

void PnBar::removeLine(PnLine *line) {
  int index = lines.indexOf(line);

  if (index != -1) lines.removeAt(index);
}

void PnBar::removeLines() {
  foreach (PnLine *line, lines) {
    line->noI()->removeLine(line);
    line->noF()->removeLine(line);
    scene()->removeItem(line);
    delete line;
  }
}

QRectF PnBar::boundingRect() const {
  return QRectF(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
}

void PnBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::gray);

  if (isSelected()) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  } else {
    painter->setBrush(Qt::blue);
    painter->drawEllipse(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  }
}

#include "pnbar.h"

#include <QGraphicsScene>
#include <QPainter>

/*******************************************************************************
 * PnBar.
 ******************************************************************************/
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
    rSlc(0, 0),
    pnInfoBar_(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);
  setZValue(1);
}

/*******************************************************************************
 * ~PnBar.
 ******************************************************************************/
PnBar::~PnBar()
{

}

/*******************************************************************************
 * Ia.
 ******************************************************************************/
complex<double> PnBar:: Ia()
{
  complex<double> I;
  I = conj(Sga - Sla) / conj(Va);
  return I;
}

/*******************************************************************************
 * Ib.
 ******************************************************************************/
complex<double> PnBar:: Ib()
{
  complex<double> I;
  I = conj(Sgb - Slb) / conj(Vb);
  return I;
}

/*******************************************************************************
 * Ic.
 ******************************************************************************/
complex<double> PnBar:: Ic()
{
  complex<double> I;
  I = conj(Sgc - Slc) / conj(Vc);
  return I;
}

/*******************************************************************************
 * rIa.
 ******************************************************************************/
complex<double> PnBar:: rIa()
{
  complex<double> I;
  I = conj(rSga - rSla) / conj(rVa);
  return I;
}

/*******************************************************************************
 * rIb.
 ******************************************************************************/
complex<double> PnBar:: rIb()
{
  complex<double> I;
  I = conj(rSgb - rSlb) / conj(rVb);
  return I;
}

/*******************************************************************************
 * rIc.
 ******************************************************************************/
complex<double> PnBar:: rIc()
{
  complex<double> I;
  I = conj(rSgc - rSlc) / conj(rVc);
  return I;
}

/*******************************************************************************
 * addLine.
 ******************************************************************************/
void PnBar::addLine(PnLine *line)
{
  lines.append(line);
}

/*******************************************************************************
 * removeLine.
 ******************************************************************************/
void PnBar::removeLine(PnLine *line)
{
  int index = lines.indexOf(line);

  if (index != -1) lines.removeAt(index);
}

/*******************************************************************************
 * removeLines.
 ******************************************************************************/
void PnBar::removeLines()
{
  foreach (PnLine *line, lines) {
    line->noI()->removeLine(line);
    line->noF()->removeLine(line);
    scene()->removeItem(line);
    delete line;
  }
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF PnBar::boundingRect() const
{
  return QRectF(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
}

/*******************************************************************************
 * mouseDoubleClickEvent.
 ******************************************************************************/
void PnBar::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event);
  emit eventDoubleClick();
}

/*******************************************************************************
 * itemChange.
 ******************************************************************************/
QVariant PnBar::itemChange(QGraphicsItem::GraphicsItemChange change,
                           const QVariant &value)
{
  if (change == ItemPositionHasChanged) {
    foreach (PnLine *line, lines) {

      line->updatePosition();
    }
  } else   if (change == ItemSelectedChange) {
    if (value == true) {
      if (pnInfoBar_ == NULL) {
        pnInfoBar_ = new PnInfoBar(this);
        scene()->addItem(pnInfoBar_);
      }
    } else {
      scene()->removeItem(pnInfoBar_);
      delete pnInfoBar_;
      pnInfoBar_ = NULL;
    }
  }

  return QGraphicsObject::itemChange(change, value);
}

/*******************************************************************************
 * paint.
 ******************************************************************************/
void PnBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(Qt::gray);

  if(id > 0)
    drawPq(painter);
  else
    drawSlack(painter);

}

/*******************************************************************************
 * drawSlack.
 ******************************************************************************/
void PnBar::drawSlack(QPainter *painter)
{
  if (isSelected()) {
    painter->setBrush(Qt::red);
    painter->drawRect(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  } else {
    painter->setBrush(Qt::green);
    painter->drawRect(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  }
}

/*******************************************************************************
 * drawPq.
 ******************************************************************************/
void PnBar::drawPq(QPainter *painter)
{
  if (isSelected()) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  } else {
    painter->setBrush(Qt::blue);
    painter->drawEllipse(-kIconSize / 2, -kIconSize / 2, kIconSize, kIconSize);
  }
}

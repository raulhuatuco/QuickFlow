#include "pnline.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

/*******************************************************************************
 * PnLine.
 ******************************************************************************/
PnLine::PnLine() :
  Zaa(qInf(),qInf()),
  Zab(qInf(),qInf()),
  Zac(qInf(),qInf()),
  Zan(qInf(),qInf()),
  Zbb(qInf(),qInf()),
  Zbc(qInf(),qInf()),
  Zbn(qInf(),qInf()),
  Zcc(qInf(),qInf()),
  Zcn(qInf(),qInf()),
  Znn(qInf(),qInf()),
  Ia(0,0),
  Ib(0,0),
  Ic(0,0),
  In(0,0),
  length(0),
  noI_(NULL),
  noF_(NULL),
  pnInfoLine_(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);
  setZValue(0.0);
}

/*******************************************************************************
 * ~PnLine.
 ******************************************************************************/
PnLine::~PnLine() {}

/*******************************************************************************
 * noI.
 ******************************************************************************/
PnBar *PnLine::noI()
{
  return noI_;
}

/*******************************************************************************
 * noF.
 ******************************************************************************/
PnBar *PnLine::noF()
{
  return noF_;
}

/*******************************************************************************
 * setNodes.
 ******************************************************************************/
void PnLine::setNodes(PnBar *noI, PnBar *noF)
{
  noI_ = noI;
  noF_ = noF;

  if ((noI != NULL) && (noF != NULL)) {
    noI->addLine(this);
    noF->addLine(this);
  }

  updatePosition();
}

/*******************************************************************************
 * Yaa.
 ******************************************************************************/
std::complex<double> PnLine::Yaa()
{
  return std::complex<double>(1.0, 0.0) / Zaa;
}

/*******************************************************************************
 * Yab.
 ******************************************************************************/
std::complex<double> PnLine::Yab()
{
  return std::complex<double>(1.0, 0.0) / Zab;
}

/*******************************************************************************
 * Yac.
 ******************************************************************************/
std::complex<double> PnLine::Yac()
{
  return std::complex<double>(1.0, 0.0) / Zac;
}

/*******************************************************************************
 * Yan.
 ******************************************************************************/
std::complex<double> PnLine::Yan()
{
  return std::complex<double>(1.0, 0.0) / Zan;
}

/*******************************************************************************
 * Ybb.
 ******************************************************************************/
std::complex<double> PnLine::Ybb()
{
  return std::complex<double>(1.0, 0.0) / Zbb;
}

/*******************************************************************************
 * Ybc.
 ******************************************************************************/
std::complex<double> PnLine::Ybc()
{
  return std::complex<double>(1.0, 0.0) / Zbc;
}

/*******************************************************************************
 * Ybn.
 ******************************************************************************/
std::complex<double> PnLine::Ybn()
{
  return std::complex<double>(1.0, 0.0) / Zbn;
}

/*******************************************************************************
 * Ycc.
 ******************************************************************************/
std::complex<double> PnLine::Ycc()
{
  return std::complex<double>(1.0, 0.0) / Zcc;
}

/*******************************************************************************
 * Ycn.
 ******************************************************************************/
std::complex<double> PnLine::Ycn()
{
  return std::complex<double>(1.0, 0.0) / Zcn;
}

/*******************************************************************************
 * Ynn.
 ******************************************************************************/
std::complex<double> PnLine::Ynn()
{
  return std::complex<double>(1.0, 0.0) / Znn;
}

/*******************************************************************************
 * La.
 ******************************************************************************/
std::complex<double> PnLine::La()
{
  return Ia * conj(Ia) * Zaa;
}

/*******************************************************************************
 * Lb.
 ******************************************************************************/
std::complex<double> PnLine::Lb()
{
  return Ib * conj(Ib) * Zbb;
}

/*******************************************************************************
 * Lc.
 ******************************************************************************/
std::complex<double> PnLine::Lc()
{
  return Ic * conj(Ic) * Zcc;
}

/*******************************************************************************
 * Ln.
 ******************************************************************************/
std::complex<double> PnLine::Ln()
{
  return In * conj(In) * Znn;
}

/*******************************************************************************
 * updatePosition.
 ******************************************************************************/
void PnLine::updatePosition()
{
  prepareGeometryChange();

  coords = QLineF(noI_->pos(), noF_->pos());

  qreal radAngle = coords.angle() * M_PI / 180.0;
  qreal dx = kCableWidth * sin(radAngle);
  qreal dy = kCableWidth * cos(radAngle);
  QPointF offset1 = QPointF(dx, dy);
  QPointF offset2 = QPointF(-dx, -dy);
  selectionArea.clear();
  selectionArea << coords.p1() + offset1 << coords.p1() + offset2
                << coords.p2() + offset2 << coords.p2() + offset1;
}

/*******************************************************************************
 * itemChange.
 ******************************************************************************/
QVariant PnLine::itemChange(QGraphicsItem::GraphicsItemChange change,
                            const QVariant &value)
{
  if (change == ItemSelectedChange) {
    if (value == true) {
      if (pnInfoLine_ == NULL) {
        pnInfoLine_ = new PnInfoLine(this);
        scene()->addItem(pnInfoLine_);
      }
    } else {
      scene()->removeItem(pnInfoLine_);
      delete pnInfoLine_;
      pnInfoLine_ = NULL;
    }
  }

  return QGraphicsObject::itemChange(change, value);
}

/*******************************************************************************
 * mouseDoubleClickEvent.
 ******************************************************************************/
void PnLine::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event);
  emit eventDoubleClick();
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF PnLine::boundingRect() const
{
  return selectionArea.boundingRect();
}

/*******************************************************************************
 * shape.
 ******************************************************************************/
QPainterPath PnLine::shape() const
{
  QPainterPath path;
  path.addPolygon(selectionArea);
  return path;
}

/*******************************************************************************
 * paint.
 ******************************************************************************/
void PnLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  if (isSelected())
    painter->setPen(QPen(Qt::red, kCableWidth, Qt::SolidLine));
  else
    painter->setPen(QPen(Qt::black, kCableWidth, Qt::SolidLine));

  painter->drawLine(coords.p1(), coords.p2());
}

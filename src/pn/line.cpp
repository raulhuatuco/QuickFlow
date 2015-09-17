#include "line.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

/*******************************************************************************
 * PnLine.
 ******************************************************************************/
PnLine::PnLine() :
  noI(0),
  noF(0),
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
  pNoI_(NULL),
  pNoF_(NULL),
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
 * pNoI.
 ******************************************************************************/
Bar *PnLine::pNoI()
{
  return pNoI_;
}

/*******************************************************************************
 * pNoF.
 ******************************************************************************/
Bar *PnLine::pNoF()
{
  return pNoF_;
}

/*******************************************************************************
 * setNodes.
 ******************************************************************************/
void PnLine::setNodes(Bar *pNoI, Bar *pNoF)
{
  pNoI_ = pNoI;
  pNoF_ = pNoF;

  if ((pNoI != NULL) && (pNoF != NULL)) {
    pNoI->addLine(this);
    pNoF->addLine(this);
  }

  updatePosition();
}

/*******************************************************************************
 * toJson.
 ******************************************************************************/
QJsonObject PnLine::toJson()
{
  // Transform line data to Json object.
  QJsonObject jsonLine;

  // Nodes
  jsonLine.insert("noI", static_cast<int>(noI));
  jsonLine.insert("noF", static_cast<int>(noF));

  // Impedance
  jsonLine.insert("Zaa", Zaa.real());
  jsonLine.insert("Zaai", Zaa.imag());
  jsonLine.insert("Zab", Zab.real());
  jsonLine.insert("Zabi", Zab.imag());
  jsonLine.insert("Zac", Zac.real());
  jsonLine.insert("Zaci", Zac.imag());
  jsonLine.insert("Zan", Zan.real());
  jsonLine.insert("Zani", Zan.imag());
  jsonLine.insert("Zbb", Zbb.real());
  jsonLine.insert("Zbbi", Zbb.imag());
  jsonLine.insert("Zbc", Zbc.real());
  jsonLine.insert("Zbci", Zbc.imag());
  jsonLine.insert("Zbn", Zbn.real());
  jsonLine.insert("Zbni", Zbn.imag());
  jsonLine.insert("Zcc", Zcc.real());
  jsonLine.insert("Zcci", Zcc.imag());
  jsonLine.insert("Zcn", Zcn.real());
  jsonLine.insert("Zcni", Zcn.imag());
  jsonLine.insert("Znn", Znn.real());
  jsonLine.insert("Znni", Znn.imag());

  // Current
  jsonLine.insert("Ia", Ia.real());
  jsonLine.insert("Iai", Ia.imag());
  jsonLine.insert("Ib", Ib.real());
  jsonLine.insert("Ibi", Ib.imag());
  jsonLine.insert("Ic", Ic.real());
  jsonLine.insert("Ici", Ic.imag());
  jsonLine.insert("In", In.real());
  jsonLine.insert("Ini", In.imag());

  return jsonLine;
}

/*******************************************************************************
 * fromJson.
 ******************************************************************************/
void PnLine::fromJson(QJsonObject &jsonLine)
{
  // Get NoI & NoF
  noI = static_cast<uint32_t>(jsonLine.value("noI").toInt());
  noF = static_cast<uint32_t>(jsonLine.value("noF").toInt());

  // Impedance
  Zaa.real(jsonLine.value("Zaa").toDouble());
  Zaa.imag(jsonLine.value("Zaai").toDouble());
  Zab.real(jsonLine.value("Zab").toDouble());
  Zab.imag(jsonLine.value("Zabi").toDouble());
  Zac.real(jsonLine.value("Zac").toDouble());
  Zac.imag(jsonLine.value("Zaci").toDouble());
  Zan.real(jsonLine.value("Zan").toDouble());
  Zan.imag(jsonLine.value("Zani").toDouble());
  Zbb.real(jsonLine.value("Zbb").toDouble());
  Zbb.imag(jsonLine.value("Zbbi").toDouble());
  Zbc.real(jsonLine.value("Zbc").toDouble());
  Zbc.imag(jsonLine.value("Zbci").toDouble());
  Zbn.real(jsonLine.value("Zbn").toDouble());
  Zbn.imag(jsonLine.value("Zbni").toDouble());
  Zcc.real(jsonLine.value("Zcc").toDouble());
  Zcc.imag(jsonLine.value("Zcci").toDouble());
  Zcn.real(jsonLine.value("Zcn").toDouble());
  Zcn.imag(jsonLine.value("Zcni").toDouble());
  Znn.real(jsonLine.value("Znn").toDouble());
  Znn.imag(jsonLine.value("Znni").toDouble());

  // Current
  Ia.real(jsonLine.value("Ia").toDouble());
  Ia.imag(jsonLine.value("Iai").toDouble());
  Ib.real(jsonLine.value("Ib").toDouble());
  Ib.imag(jsonLine.value("Ibi").toDouble());
  Ic.real(jsonLine.value("Ic").toDouble());
  Ic.imag(jsonLine.value("Ici").toDouble());
  In.real(jsonLine.value("In").toDouble());
  In.imag(jsonLine.value("Ini").toDouble());
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

  coords = QLineF(pNoI_->pos(), pNoF_->pos());

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

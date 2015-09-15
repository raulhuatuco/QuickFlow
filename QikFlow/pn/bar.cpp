#include "bar.h"

#include <QGraphicsScene>
#include <QPainter>

/*******************************************************************************
 * PnBar.
 ******************************************************************************/
PnBar::PnBar()
  : QGraphicsObject(),
    id(kInvalidId),
    pnNetwork(NULL),
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
    line->pNoI()->removeLine(line);
    line->pNoF()->removeLine(line);
    scene()->removeItem(line);
    delete line;
    }
}

void PnBar::toPerUnit()
{
  
}

void PnBar::toBaseUnit()
{
  
}

/*******************************************************************************
 * toJson.
 ******************************************************************************/
QJsonObject PnBar::toJson()
{
  // Json Object to be filled with data.
  QJsonObject jsonBar;

  // Bar Id
  jsonBar.insert("id", static_cast<int>(id));

  // Voltage
  jsonBar.insert("Va", Va.real());
  jsonBar.insert("Vai", Va.imag());
  jsonBar.insert("Vb", Vb.real());
  jsonBar.insert("Vbi", Vb.imag());
  jsonBar.insert("Vc", Vc.real());
  jsonBar.insert("Vci", Vc.imag());

  // Generated Power
  jsonBar.insert("Sga", Sga.real());
  jsonBar.insert("Sgai", Sga.imag());
  jsonBar.insert("Sgb", Sgb.real());
  jsonBar.insert("Sgbi", Sgb.imag());
  jsonBar.insert("Sgc", Sgc.real());
  jsonBar.insert("Sgci", Sgc.imag());

  // Load Power
  jsonBar.insert("Sla", Sla.real());
  jsonBar.insert("Slai", Sla.imag());
  jsonBar.insert("Slb", Slb.real());
  jsonBar.insert("Slbi", Slb.imag());
  jsonBar.insert("Slc", Slc.real());
  jsonBar.insert("Slci", Slc.imag());

  // Result Voltage
  jsonBar.insert("rVa", rVa.real());
  jsonBar.insert("rVai", rVa.imag());
  jsonBar.insert("rVb", rVb.real());
  jsonBar.insert("rVbi", rVb.imag());
  jsonBar.insert("rVc", rVc.real());
  jsonBar.insert("rVci", rVc.imag());

  // Result Generated Power
  jsonBar.insert("rSga", rSga.real());
  jsonBar.insert("rSgai", rSga.imag());
  jsonBar.insert("rSgb", rSgb.real());
  jsonBar.insert("rSgbi", rSgb.imag());
  jsonBar.insert("rSgc", rSgc.real());
  jsonBar.insert("rSgci", rSgc.imag());

  // Result Load Power
  jsonBar.insert("rSla", rSla.real());
  jsonBar.insert("rSlai", rSla.imag());
  jsonBar.insert("rSlb", rSlb.real());
  jsonBar.insert("rSlbi", rSlb.imag());
  jsonBar.insert("rSlc", rSlc.real());
  jsonBar.insert("rSlci", rSlc.imag());

  // Position
  jsonBar.insert("x", x());
  jsonBar.insert("y", y());

  return jsonBar;
}


/*******************************************************************************
 * fromJson.
 ******************************************************************************/
void PnBar::fromJson(QJsonObject &jsonBar)
{
  // Get Id
  id = static_cast<uint32_t> (jsonBar.value("id").toInt());

  // Voltage
  Va.real(jsonBar.value("Va").toDouble());
  Va.imag(jsonBar.value("Vai").toDouble());
  Vb.real(jsonBar.value("Vb").toDouble());
  Vb.imag(jsonBar.value("Vbi").toDouble());
  Vc.real(jsonBar.value("Vc").toDouble());
  Vc.imag(jsonBar.value("Vci").toDouble());

  // Generated Power
  Sga.real(jsonBar.value("Sga").toDouble());
  Sga.imag(jsonBar.value("Sgai").toDouble());
  Sgb.real(jsonBar.value("Sgb").toDouble()) ;
  Sgb.imag(jsonBar.value("Sgbi").toDouble());
  Sgc.real(jsonBar.value("Sgc").toDouble());
  Sgc.imag(jsonBar.value("Sgci").toDouble());

  // Load Power
  Sla.real(jsonBar.value("Sla").toDouble());
  Sla.imag(jsonBar.value("Slai").toDouble());
  Slb.real(jsonBar.value("Slb").toDouble());
  Slb.imag(jsonBar.value("Slbi").toDouble());
  Slc.real(jsonBar.value("Slc").toDouble());
  Slc.imag(jsonBar.value("Slci").toDouble());

  // Result Voltage
  rVa.real(jsonBar.value("rVa").toDouble());
  rVa.imag(jsonBar.value("rVai").toDouble());
  rVb.real(jsonBar.value("rVb").toDouble());
  rVb.imag(jsonBar.value("rVbi").toDouble());
  rVc.real(jsonBar.value("rVc").toDouble());
  rVc.imag(jsonBar.value("rVci").toDouble());

  // Result Generated Power
  rSga.real(jsonBar.value("rSga").toDouble());
  rSga.imag(jsonBar.value("rSgai").toDouble());
  rSgb.real(jsonBar.value("rSgb").toDouble());
  rSgb.imag(jsonBar.value("rSgbi").toDouble());
  rSgc.real(jsonBar.value("rSgc").toDouble());
  rSgc.imag(jsonBar.value("rSgci").toDouble());

  // Result Load Power
  rSla.real(jsonBar.value("rSla").toDouble());
  rSla.imag(jsonBar.value("rSlai").toDouble());
  rSlb.real(jsonBar.value("rSlb").toDouble());
  rSlb.imag(jsonBar.value("rSlbi").toDouble());
  rSlc.real(jsonBar.value("rSlc").toDouble());
  rSlc.imag(jsonBar.value("rSlci").toDouble());

  // Get Position
  setX(jsonBar.value("x").toDouble());
  setY(jsonBar.value("y").toDouble());
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

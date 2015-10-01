/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \addtogroup Models
 * \{
 */

/*!
 * \file line.cpp
 *
 * \brief Implementation of Line class.
 *
 * This is the implementation of the Line class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#include "models/line.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Line::Line() :
  noI(kInvalidNode),
  noF(kInvalidNode),
  length(0.0),
  Zaa(0.0, 0.0),
  Zab(0.0, 0.0),
  Zac(0.0, 0.0),
  Zbb(0.0, 0.0),
  Zbc(0.0, 0.0),
  Zcc(0.0, 0.0),
  Ia(0.0,0.0),
  Ib(0.0,0.0),
  Ic(0.0,0.0),
  pNoI_(NULL),
  pNoF_(NULL),
  infoLine(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);
  setZValue(0.0); // Lines are placed under bars and info boxes.
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Line::~Line() {}

/*******************************************************************************
 * pNoI.
 ******************************************************************************/
Bar *Line::pNoI()
{
  return pNoI_;
}

/*******************************************************************************
 * pNoF.
 ******************************************************************************/
Bar *Line::pNoF()
{
  return pNoF_;
}

/*******************************************************************************
 * setNodes.
 ******************************************************************************/
void Line::setNodes(Bar *pNoI, Bar *pNoF)
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
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zaa_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zaa / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zab_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zab / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zac_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zac / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zbb_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zbb / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zbc_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zbc / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
complex<double> Line::Zcc_pu(double impedanceBase)
{
  complex<double> Zpu;
  Zpu = Zcc / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * lineLoss.
 ******************************************************************************/
complex<double> Line::lossA()
{
  complex<double> loss;
  loss = Ia*Ia*(Zaa + Zab + Zac);
  return loss;
}

/*******************************************************************************
 * lineLoss.
 ******************************************************************************/
complex<double> Line::lossB()
{
  complex<double> loss;
  loss = Ib*Ib*(Zbb + Zab + Zbc);
  return loss;
}

/*******************************************************************************
 * lineLoss.
 ******************************************************************************/
complex<double> Line::lossC()
{
  complex<double> loss;
  loss = Ic*Ic*(Zcc + Zbc + Zac);
  return loss;
}

/*******************************************************************************
 * toJson.
 ******************************************************************************/
QJsonObject Line::toJson()
{
  QJsonObject jsonLine;

  // Nodes
  jsonLine.insert("noI", noI);
  jsonLine.insert("noF", noF);

  // Impedance
  jsonLine.insert("Zaa", Zaa.real());
  jsonLine.insert("Zaai", Zaa.imag());
  jsonLine.insert("Zab", Zab.real());
  jsonLine.insert("Zabi", Zab.imag());
  jsonLine.insert("Zac", Zac.real());
  jsonLine.insert("Zaci", Zac.imag());
  jsonLine.insert("Zbb", Zbb.real());
  jsonLine.insert("Zbbi", Zbb.imag());
  jsonLine.insert("Zbc", Zbc.real());
  jsonLine.insert("Zbci", Zbc.imag());
  jsonLine.insert("Zcc", Zcc.real());
  jsonLine.insert("Zcci", Zcc.imag());

  // Current
  jsonLine.insert("Ia", Ia.real());
  jsonLine.insert("Iai", Ia.imag());
  jsonLine.insert("Ib", Ib.real());
  jsonLine.insert("Ibi", Ib.imag());
  jsonLine.insert("Ic", Ic.real());
  jsonLine.insert("Ici", Ic.imag());

  // Length
  jsonLine.insert("length", length);

  return jsonLine;
}

/*******************************************************************************
 * fromJson.
 ******************************************************************************/
void Line::fromJson(QJsonObject &jsonLine)
{
  // Get NoI & NoF
  noI = jsonLine.value("noI").toInt();
  noF = jsonLine.value("noF").toInt();

  // Impedance
  Zaa.real(jsonLine.value("Zaa").toDouble());
  Zaa.imag(jsonLine.value("Zaai").toDouble());
  Zab.real(jsonLine.value("Zab").toDouble());
  Zab.imag(jsonLine.value("Zabi").toDouble());
  Zac.real(jsonLine.value("Zac").toDouble());
  Zac.imag(jsonLine.value("Zaci").toDouble());
  Zbb.real(jsonLine.value("Zbb").toDouble());
  Zbb.imag(jsonLine.value("Zbbi").toDouble());
  Zbc.real(jsonLine.value("Zbc").toDouble());
  Zbc.imag(jsonLine.value("Zbci").toDouble());
  Zcc.real(jsonLine.value("Zcc").toDouble());
  Zcc.imag(jsonLine.value("Zcci").toDouble());

  // Current
  Ia.real(jsonLine.value("Ia").toDouble());
  Ia.imag(jsonLine.value("Iai").toDouble());
  Ib.real(jsonLine.value("Ib").toDouble());
  Ib.imag(jsonLine.value("Ibi").toDouble());
  Ic.real(jsonLine.value("Ic").toDouble());
  Ic.imag(jsonLine.value("Ici").toDouble());

  // Length
  length = jsonLine.value("length").toDouble();
}

/*******************************************************************************
 * updatePosition.
 ******************************************************************************/
void Line::updatePosition()
{
  prepareGeometryChange();

  coords = QLineF(pNoI_->pos(), pNoF_->pos());

  qreal radAngle = coords.angle() * M_PI / 180.0;
  qreal dx = Network::lineWidth * sin(radAngle);
  qreal dy = Network::lineWidth * cos(radAngle);
  QPointF offset1 = QPointF(dx, dy);
  QPointF offset2 = QPointF(-dx, -dy);
  selectionArea.clear();
  selectionArea << coords.p1() + offset1 << coords.p1() + offset2
                << coords.p2() + offset2 << coords.p2() + offset1;
}

/*******************************************************************************
 * itemChange.
 ******************************************************************************/
QVariant Line::itemChange(QGraphicsItem::GraphicsItemChange change,
                          const QVariant &value)
{
  if (change == ItemSelectedChange) {
    if (value == true) {
      // if multiple items are selectec, we don't want to show the info box.
      if(!(scene()->selectedItems().size() > 1)) {
        if (infoLine == NULL) {
          infoLine = new InfoLine(this);
          scene()->addItem(infoLine);
        }
      }
    } else {
      if (infoLine != NULL) {
        scene()->removeItem(infoLine);
        delete infoLine;
        infoLine = NULL;
      }
    }
  }

  return QGraphicsObject::itemChange(change, value);
}

/*******************************************************************************
 * mouseDoubleClickEvent.
 ******************************************************************************/
void Line::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event);
  emit eventDoubleClick();
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF Line::boundingRect() const
{
  return selectionArea.boundingRect();
}

/*******************************************************************************
 * shape.
 ******************************************************************************/
QPainterPath Line::shape() const
{
  QPainterPath path;
  path.addPolygon(selectionArea);
  return path;
}

/*******************************************************************************
 * paint.
 ******************************************************************************/
void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

//  Network *network = static_cast<Network *>(parentItem());

//  if (isSelected())
//    painter->setPen(QPen(network->selectedColor, Network::lineWidth,
//                         Qt::SolidLine));
//  else
//    painter->setPen(QPen(network->lineColor, Network::lineWidth, Qt::SolidLine));

//  painter->drawLine(coords.p1(), coords.p2());

  if (isSelected())
    painter->setPen(QPen(Qt::red, Network::lineWidth,
                         Qt::SolidLine));
  else
    painter->setPen(QPen(Qt::black, Network::lineWidth, Qt::SolidLine));

  painter->drawLine(coords.p1(), coords.p2());
}

/*!
 * \}
 */


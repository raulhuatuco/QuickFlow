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
 * Const initialization.
 ******************************************************************************/
const int32_t Line::kInvalidNode = -1;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Line::Line() :
  nodes(QPair<int32_t, int32_t>(kInvalidNode, kInvalidNode)),
  network(NULL),
  length_(0.0),
  pNoI_(NULL),
  pNoF_(NULL),
  infoLine(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges);
  setZValue(0.0); // Lines are placed under bars and info boxes.

  for(int32_t k = 0; k<6; k++)
    z_[k] = qInf();

  for (int32_t k = 0; k<3; k++)
    i_[k] = 0.0;
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Line::~Line() {}

/*******************************************************************************
 * Impedance.
 ******************************************************************************/
complex<double> Line::z(int32_t index, Unit::ImpedanceUnit unit)
{
  switch (unit) {
  case Unit::kOhm:
    return z_[index];
    break;

  case Unit::kOhmPerMeter:
    return z_[index]*length();
    break;

  case Unit::kOhmPerKilometer:
    return z_[index]*length(Unit::kKiloMeter);
    break;

  case Unit::kOhmPerFeet:
    return z_[index]*length(Unit::kFeet);
    break;

  case Unit::kOhmPerMile:
    return z_[index]*length(Unit::kMile);
    break;

  default:
    return z_[index];
    break;
  }
}

/*******************************************************************************
 * Set impedance.
 ******************************************************************************/
void Line::setZ(int32_t index, complex<double> newImpedance,
                Unit::ImpedanceUnit unit)
{
  switch (unit) {
  case Unit::kOhm:
    z_[index] = newImpedance;
    break;

  case Unit::kOhmPerMeter:
    z_[index] = newImpedance*length();
    break;

  case Unit::kOhmPerKilometer:
    z_[index] = newImpedance*length(Unit::kKiloMeter);
    break;

  case Unit::kOhmPerFeet:
    z_[index] = newImpedance*length(Unit::kFeet);
    break;

  case Unit::kOhmPerMile:
    z_[index] = newImpedance*length(Unit::kMile);
    break;

  default:
    z_[index] = newImpedance;
    break;
  }

}

/*******************************************************************************
 * Impedance in per unit.
 ******************************************************************************/
complex<double> Line::zPu(int32_t index)
{
  if(network == NULL)
    return 0.0;

  return z_[index]/network->impedanceBase();
}

/*******************************************************************************
 * Current.
 ******************************************************************************/
complex<double> Line::i(int32_t phase, Unit::CurrentUnit unit)
{

}

/*******************************************************************************
 * Set current.
 ******************************************************************************/
void Line::setI(int32_t phase, complex<double> newCurrent,
                Unit::CurrentUnit unit)
{

}


/*******************************************************************************
 * Current in per unit.
 ******************************************************************************/
complex<double> Line::iPu(int32_t phase)
{
  if(network == NULL)
    return 0.0;

  return i_[phase]/network->currentBase();
}


/*******************************************************************************
 * Line loss.
 ******************************************************************************/
complex<double> Line::loss(int32_t phase)
{

}


/*******************************************************************************
 * Line length.
 ******************************************************************************/
double Line::length(Unit::LengthUnit unit)
{
  switch (unit) {
  case Unit::kMeter:
    return length_;
    break;

  case Unit::kKiloMeter:
    return length_/1000.0;
    break;

  case Unit::kFeet:
    return length_*3.28084;
    break;

  case Unit::kMile:
    return length_*0.000621371;
    break;

  default:
    return length_;
    break;
  }
}

/*******************************************************************************
 * Set line length.
 ******************************************************************************/
void Line::setLength(double newLength, Unit::LengthUnit unit)
{
  switch (unit) {
  case Unit::kMeter:
    length_ = newLength;
    break;

  case Unit::kKiloMeter:
    length_ = newLength*1000.0;
    break;

  case Unit::kFeet:
    length_ = newLength*3.28084;
    break;

  case Unit::kMile:
    length_ = newLength*0.000621371;
    break;

  default:
    length_ = newLength;
    break;
  }
}

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
 * toJson.
 ******************************************************************************/
QJsonObject Line::toJson()
{
  QJsonObject jsonLine;

  // Nodes
  jsonLine.insert("noI", nodes.first);
  jsonLine.insert("noF", nodes.second);

  // Impedance
//  jsonLine.insert("Zaa", Zaa.real());
//  jsonLine.insert("Zaai", Zaa.imag());
//  jsonLine.insert("Zab", Zab.real());
//  jsonLine.insert("Zabi", Zab.imag());
//  jsonLine.insert("Zac", Zac.real());
//  jsonLine.insert("Zaci", Zac.imag());
//  jsonLine.insert("Zbb", Zbb.real());
//  jsonLine.insert("Zbbi", Zbb.imag());
//  jsonLine.insert("Zbc", Zbc.real());
//  jsonLine.insert("Zbci", Zbc.imag());
//  jsonLine.insert("Zcc", Zcc.real());
//  jsonLine.insert("Zcci", Zcc.imag());

//  // Current
//  jsonLine.insert("Ia", Ia.real());
//  jsonLine.insert("Iai", Ia.imag());
//  jsonLine.insert("Ib", Ib.real());
//  jsonLine.insert("Ibi", Ib.imag());
//  jsonLine.insert("Ic", Ic.real());
//  jsonLine.insert("Ici", Ic.imag());

//  // Length
//  jsonLine.insert("length", length);

  return jsonLine;
}

/*******************************************************************************
 * fromJson.
 ******************************************************************************/
void Line::fromJson(QJsonObject &jsonLine)
{
  // Get NoI & NoF
  nodes.first = jsonLine.value("noI").toInt();
  nodes.second = jsonLine.value("noF").toInt();

  // Impedance
//  Zaa.real(jsonLine.value("Zaa").toDouble());
//  Zaa.imag(jsonLine.value("Zaai").toDouble());
//  Zab.real(jsonLine.value("Zab").toDouble());
//  Zab.imag(jsonLine.value("Zabi").toDouble());
//  Zac.real(jsonLine.value("Zac").toDouble());
//  Zac.imag(jsonLine.value("Zaci").toDouble());
//  Zbb.real(jsonLine.value("Zbb").toDouble());
//  Zbb.imag(jsonLine.value("Zbbi").toDouble());
//  Zbc.real(jsonLine.value("Zbc").toDouble());
//  Zbc.imag(jsonLine.value("Zbci").toDouble());
//  Zcc.real(jsonLine.value("Zcc").toDouble());
//  Zcc.imag(jsonLine.value("Zcci").toDouble());

//  // Current
//  Ia.real(jsonLine.value("Ia").toDouble());
//  Ia.imag(jsonLine.value("Iai").toDouble());
//  Ib.real(jsonLine.value("Ib").toDouble());
//  Ib.imag(jsonLine.value("Ibi").toDouble());
//  Ic.real(jsonLine.value("Ic").toDouble());
//  Ic.imag(jsonLine.value("Ici").toDouble());

  // Length
  length_ = jsonLine.value("length").toDouble();
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


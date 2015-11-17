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
#include "math_constants.h"
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
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Line::~Line()
{
  if(infoLine != NULL)
    delete infoLine;

  pNoI_->removeLine(this);
  pNoF_->removeLine(this);
}

/*******************************************************************************
 * Impedance.
 ******************************************************************************/
complex<double> Line::z(int32_t index, Unit::ImpedanceUnit unit)
{
  switch (unit) {
  case Unit::kOhm:
    return z_[index]*network->impedanceBase();
    break;

  case Unit::kOhmPerMeter:
    return z_[index]*network->impedanceBase()/length();
    break;

  case Unit::kOhmPerKilometer:
    return z_[index]*network->impedanceBase()/length(Unit::kKiloMeter);
    break;

  case Unit::kOhmPerFeet:
    return z_[index]*network->impedanceBase()/length(Unit::kFeet);
    break;

  case Unit::kOhmPerMile:
    return z_[index]*network->impedanceBase()/length(Unit::kMile);
    break;

  case Unit::kOhmPerUnit:
    return z_[index];
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
    z_[index] = newImpedance/network->impedanceBase();
    break;

  case Unit::kOhmPerMeter:
    z_[index] = newImpedance*length()/network->impedanceBase();
    break;

  case Unit::kOhmPerKilometer:
    z_[index] = newImpedance*length(Unit::kKiloMeter)/network->impedanceBase();
    break;

  case Unit::kOhmPerFeet:
    z_[index] = newImpedance*length(Unit::kFeet)/network->impedanceBase();
    break;

  case Unit::kOhmPerMile:
    z_[index] = newImpedance*length(Unit::kMile)/network->impedanceBase();
    break;

  case Unit::kOhmPerUnit:
    z_[index] = newImpedance;
    break;

  default:
    z_[index] = newImpedance;
    break;
  }

}

/*******************************************************************************
 * Current.
 ******************************************************************************/
complex<double> Line::i(int32_t phase, Unit::CurrentUnit unit)
{
  switch (unit) {
  case Unit::kAmpere:
    return i_[phase]*network->currentBase();
    break;

  case Unit::kKiloAmpere:
    return i_[phase]*network->currentBase()/1000.0;
    break;

  case Unit::kAmperePerUnit:
    return i_[phase];
    break;

  default:
    return i_[phase];
    break;
  }
}

/*******************************************************************************
 * Set current.
 ******************************************************************************/
void Line::setI(int32_t phase, complex<double> newCurrent,
                Unit::CurrentUnit unit)
{
  switch (unit) {
  case Unit::kAmpere:
    i_[phase] = newCurrent/network->currentBase();
    break;

  case Unit::kKiloAmpere:
    i_[phase] = newCurrent*1000.0/network->currentBase();
    break;

  case Unit::kAmperePerUnit:
    i_[phase] = newCurrent;
    break;

  default:
    i_[phase] = newCurrent;
    break;
  }
}

/*******************************************************************************
 * Line loss.
 ******************************************************************************/
complex<double> Line::loss(int32_t phase, Unit::PowerUnit unit)
{
  complex<double> vi, vf, loss;

  vi = pNoI()->rV(phase);
  vf = pNoF()->rV(phase);

  loss = (vi - vf)*conj(i_[phase]);

  switch (unit) {
  case Unit::kVA:
    return loss*network->powerBase();
    break;

  case Unit::kKiloVA:
    return loss*network->powerBase()/1000.0;
    break;

  case Unit::kVaPerUnit:
    return loss;
    break;

  default:
    return loss;
    break;
  }

  return 0.0;
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
    length_ = newLength/3.28084;
    break;

  case Unit::kMile:
    length_ = newLength/0.000621371;
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
  jsonLine.insert("Zaa", z_[0].real());
  jsonLine.insert("Zaai", z_[0].imag());
  jsonLine.insert("Zab", z_[1].real());
  jsonLine.insert("Zabi", z_[1].imag());
  jsonLine.insert("Zac", z_[2].real());
  jsonLine.insert("Zaci", z_[2].imag());
  jsonLine.insert("Zbb", z_[3].real());
  jsonLine.insert("Zbbi", z_[3].imag());
  jsonLine.insert("Zbc", z_[4].real());
  jsonLine.insert("Zbci", z_[4].imag());
  jsonLine.insert("Zcc", z_[5].real());
  jsonLine.insert("Zcci", z_[5].imag());

  // Current
  jsonLine.insert("Ia", i_[0].real());
  jsonLine.insert("Iai", i_[0].imag());
  jsonLine.insert("Ib", i_[1].real());
  jsonLine.insert("Ibi", i_[1].imag());
  jsonLine.insert("Ic", i_[2].real());
  jsonLine.insert("Ici", i_[2].imag());

  // Length
  jsonLine.insert("length", length_);

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
  z_[0].real(jsonLine.value("Zaa").toDouble());
  z_[0].imag(jsonLine.value("Zaai").toDouble());
  z_[1].real(jsonLine.value("Zab").toDouble());
  z_[1].imag(jsonLine.value("Zabi").toDouble());
  z_[2].real(jsonLine.value("Zac").toDouble());
  z_[2].imag(jsonLine.value("Zaci").toDouble());
  z_[3].real(jsonLine.value("Zbb").toDouble());
  z_[3].imag(jsonLine.value("Zbbi").toDouble());
  z_[4].real(jsonLine.value("Zbc").toDouble());
  z_[4].imag(jsonLine.value("Zbci").toDouble());
  z_[5].real(jsonLine.value("Zcc").toDouble());
  z_[5].imag(jsonLine.value("Zcci").toDouble());

  // Current
  i_[0].real(jsonLine.value("Ia").toDouble());
  i_[0].imag(jsonLine.value("Iai").toDouble());
  i_[1].real(jsonLine.value("Ib").toDouble());
  i_[1].imag(jsonLine.value("Ibi").toDouble());
  i_[2].real(jsonLine.value("Ic").toDouble());
  i_[2].imag(jsonLine.value("Ici").toDouble());

  // Length
  length_ = jsonLine.value("length").toDouble();
}

/*******************************************************************************
 * updatePosition.
 ******************************************************************************/
void Line::updatePosition()
{
  prepareGeometryChange();

  double yOffset = 0.0;
  double xOffset = 0.0;

  if(network != NULL) {
    xOffset = network->xOffset;
    yOffset = network->yOffset;
  }

  QPoint p1(pNoI_->x() + xOffset, pNoI_->y() + yOffset);
  QPoint p2(pNoF_->x() + xOffset, pNoF_->y() + yOffset);

  coords = QLineF(p1, p2);

  qreal radAngle = coords.angle() * kPI / 180.0;
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
      if(!(scene()->selectedItems().size() >= 1)) {
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

  if (isSelected())
    painter->setPen(QPen(network->selectedColor, Network::lineWidth,
                         Qt::SolidLine));
  else
    painter->setPen(QPen(network->lineColor, Network::lineWidth, Qt::SolidLine));

  painter->drawLine(coords.p1(), coords.p2());
}

/*!
 * \}
 */


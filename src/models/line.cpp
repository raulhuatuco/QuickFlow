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
 * Static Variables Initialization.
 ******************************************************************************/
double Line::lineWidth = 4.0;
QColor Line::lineColor = Qt::black;
QColor Line::selectedLineColor = Qt::red;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Line::Line() :
  noI(kInvalidNode),
  noF(kInvalidNode),
  length(0.0),
  pNoI_(NULL),
  pNoF_(NULL),
  Z_(3,3, 0.0),
  I_(3, 0.0),
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
 * Get Impedance.
 ******************************************************************************/
Mat<cx_double> &Line::Z() const
{
  return &Z_;
}

/*******************************************************************************
 * setZaa.
 ******************************************************************************/
void Line::setZaa(cx_double Zaa)
{
  Z_(0,0) = Zaa;
}

/*******************************************************************************
 * setZab.
 ******************************************************************************/
void Line::setZab(cx_double Zab)
{
  Z_(0,1) = Zab;
}

/*******************************************************************************
 * setZac.
 ******************************************************************************/
void Line::setZac(cx_double Zac)
{
  Z_(0,2) = Zac;
}

/*******************************************************************************
 * setZbb.
 ******************************************************************************/
void Line::setZbb(cx_double Zbb)
{
  Z_(1,1) = Zbb;
}

/*******************************************************************************
 * setZbc.
 ******************************************************************************/
void Line::setZbc(cx_double Zbc)
{
  Z_(1,2) = Zbc;
}

/*******************************************************************************
 * setZcc.
 ******************************************************************************/
void Line::setZcc(cx_double Zcc)
{
  Z_(2,2) = Zcc;
}

/*******************************************************************************
 * Get Impedance in pu.
 ******************************************************************************/
Mat<cx_double> Line::Z_pu(double impedanceBase) const
{
  Mat<cx_double> Zpu(3,3);
  Zpu = Z_ / impedanceBase;
  return Zpu;
}

/*******************************************************************************
 * Current.
 ******************************************************************************/
Col<cx_double> &Line::I() const
{
  return &I_;
}

/*******************************************************************************
 * Set Current.
 ******************************************************************************/
void Line::setI(cx_double &Ia, cx_double &Ib, cx_double &Ic)
{
  I_[0] = Ia;
  I_[1] = Ib;
  I_[2] = Ic;
}

/*******************************************************************************
 * lineLoss.
 ******************************************************************************/
Col<cx_double> Line::lineLoss()
{
  Col<cx_double> loss(3);
  loss[0] = I_[0]*I_[0]*(Z_(0,0) + Z_(0,1) + Z_(0,2));
  loss[1] = I_[1]*I_[1]*(Z_(1,0) + Z_(1,1) + Z_(1,2));
  loss[2] = I_[2]*I_[2]*(Z_(2,0) + Z_(2,1) + Z_(2,2));
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
  jsonLine.insert("Zaa", Z_(0,0).real());
  jsonLine.insert("Zaai", Z_(0,0).imag());
  jsonLine.insert("Zab", Z_(0,1).real());
  jsonLine.insert("Zabi", Z_(0,1).imag());
  jsonLine.insert("Zac", Z_(0,2).real());
  jsonLine.insert("Zaci", Z_(0,2).imag());
  jsonLine.insert("Zbb", Z_(1,1).real());
  jsonLine.insert("Zbbi", Z_(1,1).imag());
  jsonLine.insert("Zbc", Z_(1,2).real());
  jsonLine.insert("Zbci", Z_(1,2).imag());
  jsonLine.insert("Zcc", Z_(2,2).real());
  jsonLine.insert("Zcci", Z_(2,2).imag());

  // Current
  jsonLine.insert("Ia", I_[0].real());
  jsonLine.insert("Iai", I_[0].imag());
  jsonLine.insert("Ib", I_[1].real());
  jsonLine.insert("Ibi", I_[1].imag());
  jsonLine.insert("Ic", I_[2].real());
  jsonLine.insert("Ici", I_[2].imag());

  // Length
  jsonLine.insert("length", length);

  // Edges
  /// TODO: Implement edges array.
//  foreach (QPointF edge, edges) {

//    }

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
  Z_(0,0).real(jsonLine.value("Zaa").toDouble());
  Z_(0,0).imag(jsonLine.value("Zaai").toDouble());
  Z_(0,1).real(jsonLine.value("Zab").toDouble());
  Z_(0,1).imag(jsonLine.value("Zabi").toDouble());
  Z_(0,2).real(jsonLine.value("Zac").toDouble());
  Z_(0,2).imag(jsonLine.value("Zaci").toDouble());
  Z_(1,1).real(jsonLine.value("Zbb").toDouble());
  Z_(1,1).imag(jsonLine.value("Zbbi").toDouble());
  Z_(1,2).real(jsonLine.value("Zbc").toDouble());
  Z_(1,2).imag(jsonLine.value("Zbci").toDouble());
  Z_(2,2).real(jsonLine.value("Zcc").toDouble());
  Z_(2,2).imag(jsonLine.value("Zcci").toDouble());

  // Current
  I_[0].real(jsonLine.value("Ia").toDouble());
  I_[0].imag(jsonLine.value("Iai").toDouble());
  I_[1].real(jsonLine.value("Ib").toDouble());
  I_[1].imag(jsonLine.value("Ibi").toDouble());
  I_[2].real(jsonLine.value("Ic").toDouble());
  I_[2].imag(jsonLine.value("Ici").toDouble());

  // Length
  length = jsonLine.value("length").toDouble();

  /// TODO: Implement edges array.
}

/*******************************************************************************
 * updatePosition.
 ******************************************************************************/
void Line::updatePosition()
{
  prepareGeometryChange();

  coords = QLineF(pNoI_->pos(), pNoF_->pos());

  qreal radAngle = coords.angle() * M_PI / 180.0;
  qreal dx =lineWidth * sin(radAngle);
  qreal dy = lineWidth * cos(radAngle);
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
      if(scene()->selectedItems().size() == 1)
        if (infoLine == NULL) {
          infoLine = new InfoLine(this);
          scene()->addItem(infoLine);
        }
    } else {
      scene()->removeItem(infoLine);
      delete infoLine;
      infoLine = NULL;
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

  if (isSelected())
    painter->setPen(QPen(selectedLineColor, lineWidth, Qt::SolidLine));
  else
    painter->setPen(QPen(lineColor, lineWidth, Qt::SolidLine));

  painter->drawLine(coords.p1(), coords.p2());
}

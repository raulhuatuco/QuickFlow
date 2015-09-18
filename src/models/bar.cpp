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
 * \file bar.cpp
 *
 * \brief Implementation of Bar class.
 *
 * This is the implementation of the Bar class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#include "models/bar.h"
#include <QGraphicsScene>
#include <QPainter>

/*******************************************************************************
 * Static Variables Initialization.
 ******************************************************************************/
double Bar::iconSize = 15.0;
QColor Bar::contourColor = Qt::gray;
QColor Bar::slackFillColor = Qt::green;
QColor Bar::pqFillColor = Qt::blue;
QColor Bar::selectionFillColor = Qt::red;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Bar::Bar()
  : QGraphicsObject(),
    id(kInvalidId),
    V_(3, cx_double(0.0,0.0)),
    Sh_(3, cx_double(0.0,0.0)),
    Si_(3, cx_double(0.0,0.0)),
    rV_(3, cx_double(0.0,0.0)),
    rSi_(3, cx_double(0.0,0.0)),
    infoBar(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges); // Needed to refresh line drawing.
  setZValue(1.0); // Will be above lines and under info boxes.
}

/*******************************************************************************
 * Constructor with initial conditions.
 ******************************************************************************/
Bar::Bar(cx_double initialV, cx_double initialSh = cx_double(0.0,0.0),
         cx_double initialSi = cx_double(0.0,0.0))
  : QGraphicsObject(),
    id(kInvalidId),
    V_(3, initialV),
    Sh_(3, initialSh),
    Si_(3, initialSi),
    rV_(3, cx_double(0.0,0.0)),
    rSi_(3, cx_double(0.0,0.0)),
    infoBar(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges); // Needed to refresh line drawing.
  setZValue(1.0); // Will be above lines and under info boxes.
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Bar::~Bar()
{
}

/*******************************************************************************
 * Get Initial Voltage.
 ******************************************************************************/
Col<cx_double> &Bar::V() const
{
  return &V_;
}

/*******************************************************************************
 * Set Initial Voltage.
 ******************************************************************************/
void Bar::setV(cx_double &VA, cx_double &VB, cx_double &VC)
{
  V_[0] = VA;
  V_[1] = VB;
  V_[2] = VC;
}

/*******************************************************************************
 * Get Initial Voltage converted to Per Unit.
 ******************************************************************************/
Col<cx_double> Bar::V_pu(double voltageBase) const
{
  Col<cx_double> Vpu(3, 0.0,0.0);
  Vpu = V_ / voltageBase;
  return Vpu;
}

/*******************************************************************************
 * Get Initial Shunt Element Power.
 ******************************************************************************/
Col<cx_double> &Bar::Sh() const
{
  return &Sh_;
}

/*******************************************************************************
 * Set Initial Shunt Element Power.
 ******************************************************************************/
void Bar::setSh(cx_double &ShuntA, cx_double &ShuntB, cx_double &ShuntC)
{
  Sh_[0] = ShuntA;
  Sh_[1] = ShuntB;
  Sh_[2] = ShuntC;
}

/*******************************************************************************
 * Get Initial Shunt Element Power converted to Per Unit.
 ******************************************************************************/
Col<cx_double> Bar::Sh_pu(double powerBase) const
{
  Col<cx_double> Shpu(3, cx_double(0.0,0.0));
  Shpu = Sh_ / powerBase;
  return Shpu;
}

/*******************************************************************************
 * Get Initial Power Injected.
 ******************************************************************************/
Col<cx_double> &Bar::Si() const
{
  return &Si_;
}

/*******************************************************************************
 * Set Initial Power Injected.
 ******************************************************************************/
void Bar::setSi(cx_double &SinjectA, cx_double &SinjectB, cx_double &SinjectC)
{
  Si_[0] = SinjectA;
  Si_[1] = SinjectB;
  Si_[2] = SinjectC;
}

/*******************************************************************************
 * Get Initial  Power Injected converted to Per Unit.
 ******************************************************************************/
Col<cx_double> Bar::Si_pu(double powerBase) const
{
  Col<cx_double> Sipu(3, cx_double(0.0,0.0));
  Sipu = Si_ / powerBase;
  return Sipu;
}

/*******************************************************************************
 * Get Resulting Voltage.
 ******************************************************************************/
Col<cx_double> &Bar::rV() const
{
  return &rV_;
}

/*******************************************************************************
 * Set Resulting Voltage.
 ******************************************************************************/
void Bar::setrV(cx_double &rVA, cx_double &rVB, cx_double &rVC)
{
  rV_[0] = rVA;
  rV_[1] = rVB;
  rV_[2] = rVC;
}

/*******************************************************************************
 * Get Resulting Voltage converted to Per Unit.
 ******************************************************************************/
Col<cx_double> Bar::rV_pu(double voltageBase) const
{
  Col<cx_double> rVpu(3, cx_double(0.0,0.0));
  rVpu = rV_ / voltageBase;
  return rVpu;
}

/*******************************************************************************
 * Get Resulting Injected Power.
 ******************************************************************************/
Col<cx_double> &Bar::rSi() const
{
  return &rSi_;
}

/*******************************************************************************
 * Set Resulting Injected Power.
 ******************************************************************************/
void Bar::setrSi(cx_double &rSinjectA, cx_double &rSinjectB,
                 cx_double &rSinjectC)
{
  rSi_[0] = rSinjectA;
  rSi_[1] = rSinjectB;
  rSi_[2] = rSinjectC;
}

/*******************************************************************************
 * Get Resulting Injected Power converted to Per Unit.
 ******************************************************************************/
Col<cx_double> Bar::rSi_pu(double powerBase) const
{
  Col<cx_double> rSipu(3, cx_double(0.0,0.0));
  rSipu = rSi_ / powerBase;
  return rSipu;
}

/*******************************************************************************
 * addLine.
 ******************************************************************************/
void Bar::addLine(Line *line)
{
  lines.append(line);
}

/*******************************************************************************
 * removeLine.
 ******************************************************************************/
void Bar::removeLine(Line *line)
{
  int index = lines.indexOf(line);

  if (index != -1) lines.removeAt(index);
}

/*******************************************************************************
 * removeLines.
 ******************************************************************************/
void Bar::removeLines()
{
  foreach (Line *line, lines) {
    line->pNoI()->removeLine(line);
    line->pNoF()->removeLine(line);
    scene()->removeItem(line);
    delete line;
  }
}

/*******************************************************************************
 * toJson.
 ******************************************************************************/
QJsonObject Bar::toJson()
{
  // Json Object to be filled with data.
  QJsonObject jsonBar;

  // Bar Id.
  jsonBar.insert("id", id);

  // Voltage.
  jsonBar.insert("Va", V_[0].real());
  jsonBar.insert("Vai", V_[0].imag());
  jsonBar.insert("Vb", V_[1].real());
  jsonBar.insert("Vbi", V_[1].imag());
  jsonBar.insert("Vc", V_[2].real());
  jsonBar.insert("Vci", V_[2].imag());

  // Shunt element power.
  jsonBar.insert("Sha", Sh_[0].real());
  jsonBar.insert("Shai", Sh_[0].imag());
  jsonBar.insert("Shb", Sh_[1].real());
  jsonBar.insert("Shbi", Sh_[1].imag());
  jsonBar.insert("Shc", Sh_[2].real());
  jsonBar.insert("Shci", Sh_[2].imag());

  // Injected Power.
  jsonBar.insert("Sia", Si_[0].real());
  jsonBar.insert("Siai", Si_[0].imag());
  jsonBar.insert("Sib", Si_[1].real());
  jsonBar.insert("Sibi", Si_[1].imag());
  jsonBar.insert("Sic", Si_[2].real());
  jsonBar.insert("Sici", Si_[2].imag());

  // Result Voltage.
  jsonBar.insert("rVa", rV_[0].real());
  jsonBar.insert("rVai", rV_[0].imag());
  jsonBar.insert("rVb", rV_[1].real());
  jsonBar.insert("rVbi", rV_[1].imag());
  jsonBar.insert("rVc", rV_[2].real());
  jsonBar.insert("rVci", rV_[2].imag());

  // Result Injected Power.
  jsonBar.insert("rSia", rSi_[0].real());
  jsonBar.insert("rSiai", rSi_[0].imag());
  jsonBar.insert("rSib", rSi_[1].real());
  jsonBar.insert("rSibi", rSi_[1].imag());
  jsonBar.insert("rSic", rSi_[2].real());
  jsonBar.insert("rSici", rSi_[2].imag());

  // Position.
  jsonBar.insert("x", x());
  jsonBar.insert("y", y());

  return jsonBar;
}

/*******************************************************************************
 * fromJson.
 ******************************************************************************/
void Bar::fromJson(QJsonObject &jsonBar)
{
  // Get Id.
  id = jsonBar.value("id").toInt();

  // Voltage.
  V_[0].real(jsonBar.value("Va").toDouble());
  V_[0].imag(jsonBar.value("Vai").toDouble());
  V_[1].real(jsonBar.value("Vb").toDouble());
  V_[1].imag(jsonBar.value("Vbi").toDouble());
  V_[2].real(jsonBar.value("Vc").toDouble());
  V_[2].imag(jsonBar.value("Vci").toDouble());

  // Shunt element power.
  Sh_[0].real(jsonBar.value("Sha").toDouble());
  Sh_[0].imag(jsonBar.value("Shai").toDouble());
  Sh_[1].real(jsonBar.value("Shb").toDouble()) ;
  Sh_[1].imag(jsonBar.value("Shbi").toDouble());
  Sh_[2].real(jsonBar.value("Shc").toDouble());
  Sh_[2].imag(jsonBar.value("Shci").toDouble());

  // Injected Power.
  Si_[0].real(jsonBar.value("Sia").toDouble());
  Si_[0].imag(jsonBar.value("Siai").toDouble());
  Si_[1].real(jsonBar.value("Sib").toDouble());
  Si_[1].imag(jsonBar.value("Sibi").toDouble());
  Si_[2].real(jsonBar.value("Sic").toDouble());
  Si_[2].imag(jsonBar.value("Sici").toDouble());

  // Result Voltage.
  rV_[0].real(jsonBar.value("rVa").toDouble());
  rV_[0].imag(jsonBar.value("rVai").toDouble());
  rV_[1].real(jsonBar.value("rVb").toDouble());
  rV_[1].imag(jsonBar.value("rVbi").toDouble());
  rV_[2].real(jsonBar.value("rVc").toDouble());
  rV_[2].imag(jsonBar.value("rVci").toDouble());

  // Result Injected Power.
  rSi_[0].real(jsonBar.value("rSga").toDouble());
  rSi_[0].imag(jsonBar.value("rSgai").toDouble());
  rSi_[0].real(jsonBar.value("rSgb").toDouble());
  rSi_[0].imag(jsonBar.value("rSgbi").toDouble());
  rSi_[0].real(jsonBar.value("rSgc").toDouble());
  rSi_[0].imag(jsonBar.value("rSgci").toDouble());

  // Get Position.
  setX(jsonBar.value("x").toDouble());
  setY(jsonBar.value("y").toDouble());
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF Bar::boundingRect() const
{
  return QRectF(-iconSize / 2, -iconSize / 2, iconSize, iconSize);
}

/*******************************************************************************
 * mouseDoubleClickEvent.
 ******************************************************************************/
void Bar::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED(event);
  emit eventDoubleClick();
}

/*******************************************************************************
 * itemChange.
 ******************************************************************************/
QVariant Bar::itemChange(QGraphicsItem::GraphicsItemChange change,
                         const QVariant &value)
{
  // if bar moved,update lines to the current position.
  if (change == ItemPositionHasChanged) {
    foreach (Line *line, lines) {
      line->updatePosition();
    }
  }
  // if bar is selected, show information box.
  else if (change == ItemSelectedChange) {
    if (value == true) {
      // if multiple bars are selectec, we don't want to show the info box.
      if(scene()->selectedItems().size() == 1) {
        // Check if box already exists.
        if (infoBar == NULL) {
          // Create a new box.
          infoBar = new InfoBar(this);
          scene()->addItem(infoBar);
        }
      }
    }
    // Box has been deselected.
    else {
      scene()->removeItem(infoBar);
      delete infoBar;
      infoBar = NULL;
    }
  }

  return QGraphicsObject::itemChange(change, value);
}

/*******************************************************************************
 * paint.
 ******************************************************************************/
void Bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->setPen(contourColor);

  if(id > 0)
    drawPq(painter);
  else
    drawSlack(painter);

}

/*******************************************************************************
 * drawSlack.
 ******************************************************************************/
void Bar::drawSlack(QPainter *painter)
{
  if (isSelected()) {
    painter->setBrush(selectionFillColor);
  } else {
    painter->setBrush(slackFillColor);
  }

  painter->drawRect(-iconSize / 2, -iconSize / 2, iconSize, iconSize);
}

/*******************************************************************************
 * drawPq.
 ******************************************************************************/
void Bar::drawPq(QPainter *painter)
{
  if (isSelected()) {
    painter->setBrush(selectionFillColor);
  } else {
    painter->setBrush(pqFillColor);
  }

  painter->drawEllipse(-iconSize / 2, -iconSize / 2, iconSize, iconSize);
}

/*!
 * \}
 */

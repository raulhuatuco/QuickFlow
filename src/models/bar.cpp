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
 * Constructor.
 ******************************************************************************/
Bar::Bar()
  : QGraphicsObject(),
    id(kInvalidId),
    Va(0.0),
    Vb(0.0),
    Vc(0.0),
    Sha(0.0),
    Shb(0.0),
    Shc(0.0),
    Sia(0.0),
    Sib(0.0),
    Sic(0.0),
    rVa(0.0),
    rVb(0.0),
    rVc(0.0),
    rSia(0.0),
    rSib(0.0),
    rSic(0.0),
    infoBar(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges); // Needed to refresh line drawing.
  setZValue(1.0); // Will be above lines and under info boxes.
}

/*******************************************************************************
 * Constructor with initial conditions.
 ******************************************************************************/
Bar::Bar(cx_double initialV, cx_double initialSh,
         cx_double initialSi)
  : QGraphicsObject(),
    id(kInvalidId),
    Va(initialV),
    Vb(initialV),
    Vc(initialV),
    Sha(initialSh),
    Shb(initialSh),
    Shc(initialSh),
    Sia(initialSi),
    Sib(initialSi),
    Sic(initialSi),
    rVa(0.0),
    rVb(0.0),
    rVc(0.0),
    rSia(0.0),
    rSib(0.0),
    rSic(0.0),
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
 * Get Initial Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Va_pu(double voltageBase)
{
  cx_double Vapu;
  Vapu = Va / voltageBase;
  return Vapu;
}

/*******************************************************************************
 * Get Initial Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Vb_pu(double voltageBase)
{
  cx_double Vbpu;
  Vbpu = Vb / voltageBase;
  return Vbpu;
}

/*******************************************************************************
 * Get Initial Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Vc_pu(double voltageBase)
{
  cx_double Vcpu;
  Vcpu = Vc / voltageBase;
  return Vcpu;
}

/*******************************************************************************
 * Get Initial Shunt Element Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Sha_pu(double powerBase)
{
  cx_double Shpu;
  Shpu = Sha / powerBase;
  return Shpu;
}

/*******************************************************************************
 * Get Initial Shunt Element Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Shb_pu(double powerBase)
{
  cx_double Shpu;
  Shpu = Shb / powerBase;
  return Shpu;
}

/*******************************************************************************
 * Get Initial Shunt Element Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Shc_pu(double powerBase)
{
  cx_double Shpu;
  Shpu = Shc / powerBase;
  return Shpu;
}

/*******************************************************************************
 * Get Initial  Power Injected converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Sia_pu(double powerBase)
{
  cx_double Sipu;
  Sipu = Sia / powerBase;
  return Sipu;
}

/*******************************************************************************
 * Get Initial  Power Injected converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Sib_pu(double powerBase)
{
  cx_double Sipu;
  Sipu = Sib / powerBase;
  return Sipu;
}

/*******************************************************************************
 * Get Initial  Power Injected converted to Per Unit.
 ******************************************************************************/
cx_double Bar::Sic_pu(double powerBase)
{
  cx_double Sipu;
  Sipu = Sic / powerBase;
  return Sipu;
}

/*******************************************************************************
 * Get Resulting Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rVa_pu(double voltageBase)
{
  cx_double rVpu;
  rVpu = rVa / voltageBase;
  return rVpu;
}

/*******************************************************************************
 * Get Resulting Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rVb_pu(double voltageBase)
{
  cx_double rVpu;
  rVpu = rVb / voltageBase;
  return rVpu;
}

/*******************************************************************************
 * Get Resulting Voltage converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rVc_pu(double voltageBase)
{
  cx_double rVpu;
  rVpu = rVc / voltageBase;
  return rVpu;
}

/*******************************************************************************
 * Get Resulting Injected Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rSia_pu(double powerBase)
{
  cx_double rSipu(3);
  rSipu = rSia / powerBase;
  return rSipu;
}

/*******************************************************************************
 * Get Resulting Injected Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rSib_pu(double powerBase)
{
  cx_double rSipu(3);
  rSipu = rSib / powerBase;
  return rSipu;
}

/*******************************************************************************
 * Get Resulting Injected Power converted to Per Unit.
 ******************************************************************************/
cx_double Bar::rSic_pu(double powerBase)
{
  cx_double rSipu(3);
  rSipu = rSic / powerBase;
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
  jsonBar.insert("Va", Va.real());
  jsonBar.insert("Vai", Vb.imag());
  jsonBar.insert("Vb", Vc.real());
  jsonBar.insert("Vbi", Vb.imag());
  jsonBar.insert("Vc", Vc.real());
  jsonBar.insert("Vci", Vc.imag());

  // Shunt element power.
  jsonBar.insert("Sha", Sha.real());
  jsonBar.insert("Shai", Sha.imag());
  jsonBar.insert("Shb", Shb.real());
  jsonBar.insert("Shbi", Shb.imag());
  jsonBar.insert("Shc", Shc.real());
  jsonBar.insert("Shci", Shc.imag());

  // Injected Power.
  jsonBar.insert("Sia", Sia.real());
  jsonBar.insert("Siai", Sia.imag());
  jsonBar.insert("Sib", Sib.real());
  jsonBar.insert("Sibi", Sib.imag());
  jsonBar.insert("Sic", Sic.real());
  jsonBar.insert("Sici", Sic.imag());

  // Result Voltage.
  jsonBar.insert("rVa", rVa.real());
  jsonBar.insert("rVai", rVa.imag());
  jsonBar.insert("rVb", rVb.real());
  jsonBar.insert("rVbi", rVb.imag());
  jsonBar.insert("rVc", rVc.real());
  jsonBar.insert("rVci", rVc.imag());

  // Result Injected Power.
  jsonBar.insert("rSia", rSia.real());
  jsonBar.insert("rSiai", rSia.imag());
  jsonBar.insert("rSib", rSib.real());
  jsonBar.insert("rSibi", rSib.imag());
  jsonBar.insert("rSic", rSic.real());
  jsonBar.insert("rSici", rSic.imag());

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
  Va.real(jsonBar.value("Va").toDouble());
  Va.imag(jsonBar.value("Vai").toDouble());
  Vb.real(jsonBar.value("Vb").toDouble());
  Vb.imag(jsonBar.value("Vbi").toDouble());
  Vc.real(jsonBar.value("Vc").toDouble());
  Vc.imag(jsonBar.value("Vci").toDouble());

  // Shunt element power.
  Sha.real(jsonBar.value("Sha").toDouble());
  Sha.imag(jsonBar.value("Shai").toDouble());
  Shb.real(jsonBar.value("Shb").toDouble()) ;
  Shb.imag(jsonBar.value("Shbi").toDouble());
  Shc.real(jsonBar.value("Shc").toDouble());
  Shc.imag(jsonBar.value("Shci").toDouble());

  // Injected Power.
  Sia.real(jsonBar.value("Sia").toDouble());
  Sia.imag(jsonBar.value("Siai").toDouble());
  Sib.real(jsonBar.value("Sib").toDouble());
  Sib.imag(jsonBar.value("Sibi").toDouble());
  Sic.real(jsonBar.value("Sic").toDouble());
  Sic.imag(jsonBar.value("Sici").toDouble());

  // Result Voltage.
  rVa.real(jsonBar.value("rVa").toDouble());
  rVa.imag(jsonBar.value("rVai").toDouble());
  rVb.real(jsonBar.value("rVb").toDouble());
  rVb.imag(jsonBar.value("rVbi").toDouble());
  rVc.real(jsonBar.value("rVc").toDouble());
  rVc.imag(jsonBar.value("rVci").toDouble());

  // Result Injected Power.
  rSia.real(jsonBar.value("rSga").toDouble());
  rSia.imag(jsonBar.value("rSgai").toDouble());
  rSia.real(jsonBar.value("rSgb").toDouble());
  rSia.imag(jsonBar.value("rSgbi").toDouble());
  rSia.real(jsonBar.value("rSgc").toDouble());
  rSia.imag(jsonBar.value("rSgci").toDouble());

  // Get Position.
  setX(jsonBar.value("x").toDouble());
  setY(jsonBar.value("y").toDouble());
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF Bar::boundingRect() const
{
  return QRectF(-Network::barIconSize / 2, -Network::barIconSize / 2,
                Network::barIconSize, Network::barIconSize);
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
      if(!(scene()->selectedItems().size() > 1)) {
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
      if (infoBar != NULL) {
        scene()->removeItem(infoBar);
        delete infoBar;
        infoBar = NULL;
      }
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
  Network *network = dynamic_cast<Network *>(scene());

  painter->setPen(network->barStrokeColor);

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
  Network *network = dynamic_cast<Network *>(scene());

  if (isSelected()) {
    painter->setBrush(network->selectedColor);
  } else {
    painter->setBrush(network->barSlackFillColor);
  }

  painter->drawRect(boundingRect());
}

/*******************************************************************************
 * drawPq.
 ******************************************************************************/
void Bar::drawPq(QPainter *painter)
{

  Network *network = dynamic_cast<Network *>(scene());

  if (isSelected()) {
    painter->setBrush(network->selectedColor);
  } else {
    painter->setBrush(network->barPqFillColor);
  }

  painter->drawEllipse(boundingRect());
}

/*!
 * \}
 */

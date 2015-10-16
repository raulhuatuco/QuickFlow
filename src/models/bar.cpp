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
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#include "models/bar.h"
#include <QGraphicsScene>
#include <QPainter>

/*******************************************************************************
 * Const initialization.
 ******************************************************************************/
const int32_t Bar::kInvalidId = -1;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Bar::Bar()
  : QGraphicsObject(),
    id(kInvalidId),
    network(NULL),
    infoBar(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges); // Needed to refresh line drawing.
  setZValue(1.0); // Will be above lines and under info boxes.

  // zero fill data.
  for (int32_t i = 0; i<=2; i++) {
    v_[i] = 0.0;
    sh_[i] = 0.0;
    si_[i] = 0.0;
    rV_[i] = 0.0;
    rSi_[i] = 0.0;
  }
}

/*******************************************************************************
 * Constructor with initial conditions.
 ******************************************************************************/
Bar::Bar(complex<double> initialV, complex<double> initialSh,
         complex<double> initialSi)
  : QGraphicsObject(),
    id(kInvalidId),
    network(NULL),
    infoBar(NULL)
{
  setFlag(ItemIsSelectable);
  setFlag(ItemSendsGeometryChanges); // Needed to refresh line drawing.
  setZValue(1.0); // Will be above lines and under info boxes.

  // Fill initial conditions.
  for (int32_t i = 0; i<=2; i++) {
    v_[i] = initialV;
    sh_[i] = initialSh;
    si_[i] = initialSi;
    // Results are still zero.
    rV_[i] = 0.0;
    rSi_[i] = 0.0;
  }
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Bar::~Bar()
{
}

/*******************************************************************************
 * Initial Voltage.
 ******************************************************************************/
complex<double> Bar::v(int32_t phase, Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    return v_[phase];
    break;

  case Unit::kKiloVolts:
    return v_[phase]/1000.0;
    break;

  case Unit::kVoltsPerUnit:
    return v_[phase]/network->voltageBase;
    break;

  default:
    return v_[phase];
    break;
  }
}

/*******************************************************************************
 * Set initial Voltage.
 ******************************************************************************/
void Bar::setV(int32_t phase, complex<double> newVoltage,
               Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    v_[phase] = newVoltage;
    break;

  case Unit::kKiloVolts:
    v_[phase] = newVoltage*1000.0;
    break;

  case Unit::kVoltsPerUnit:
    v_[phase] = newVoltage*network->voltageBase;
    break;

  default:
    v_[phase] = newVoltage;
    break;
  }
}

/*******************************************************************************
 * Get Shunt Element Power.
 ******************************************************************************/
complex<double> Bar::sh(int32_t phase, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    return sh_[phase];
    break;

  case Unit::kKiloVA:
    return sh_[phase]/1000.0;
    break;

  case Unit::kMegaVa:
    return sh_[phase]/1000000.0;
    break;

  case Unit::kVaPerUnit:
    return sh_[phase]/network->powerBase;
    break;

  default:
    return sh_[phase];
    break;
  }
}

/*******************************************************************************
 * Set Shunt Element Power.
 ******************************************************************************/
void Bar::setSh(int32_t phase, complex<double> newPower, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    sh_[phase] = newPower;
    break;

  case Unit::kKiloVA:
    sh_[phase] = newPower*1000.0;
    break;

  case Unit::kMegaVa:
    sh_[phase] = newPower*1000000.0;
    break;

  case Unit::kVaPerUnit:
    sh_[phase] = newPower*network->powerBase;
    break;

  default:
    sh_[phase] = newPower;
    break;
  }
}

/*******************************************************************************
 * Injected power.
 ******************************************************************************/
complex<double> Bar::si(int32_t phase, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    return si_[phase];
    break;

  case Unit::kKiloVA:
    return si_[phase]/1000.0;
    break;

  case Unit::kMegaVa:
    return si_[phase]/1000000.0;
    break;

  case Unit::kVaPerUnit:
    return si_[phase]/network->powerBase;
    break;

  default:
    return si_[phase];
    break;
  }
}

/*******************************************************************************
 * Set Injected power.
 ******************************************************************************/
void Bar::setSi(int32_t phase, complex<double> newPower, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    si_[phase] = newPower;
    break;

  case Unit::kKiloVA:
    si_[phase] = newPower*1000.0;
    break;

  case Unit::kMegaVa:
    si_[phase] = newPower*1000000.0;
    break;

  case Unit::kVaPerUnit:
    si_[phase] = newPower*network->powerBase;
    break;

  default:
    si_[phase] = newPower;
    break;
  }
}

/*******************************************************************************
 * Result voltage.
 ******************************************************************************/
complex<double> Bar::rV(int32_t phase, Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    return rV_[phase];
    break;

  case Unit::kKiloVolts:
    return rV_[phase]/1000.0;
    break;

  case Unit::kVoltsPerUnit:
    return rV_[phase]/network->voltageBase;
    break;

  default:
    return rV_[phase];
    break;
  }
}

/*******************************************************************************
 * Set result voltage.
 ******************************************************************************/
void Bar::setRV(int32_t phase, complex<double> resultVoltage,
                Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    rV_[phase] = resultVoltage;
    break;

  case Unit::kKiloVolts:
    rV_[phase] = resultVoltage*1000.0;
    break;

  case Unit::kVoltsPerUnit:
    rV_[phase] = resultVoltage*network->voltageBase;
    break;

  default:
    rV_[phase] = resultVoltage;
    break;
  }
}

/*******************************************************************************
 * Result value for bar current.
 ******************************************************************************/
complex<double> Bar::rI(int32_t phase, Unit::CurrentUnit unit)
{
  complex<double> i;

  i = conj(si_[phase]/rV_[phase]) - conj(sh_[phase]/rV_[phase]);

  switch (unit) {
  case Unit::kAmpere:
    return i;
    break;

  case Unit::kKiloAmpere:
    return i/1000.0;
    break;

  case Unit::kAmperePerUnit:
    return i/network->currentBase();
    break;

  default:
    return i;
    break;
  }
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
  jsonBar.insert("Va", v_[0].real());
  jsonBar.insert("Vai", v_[0].imag());
  jsonBar.insert("Vb", v_[1].real());
  jsonBar.insert("Vbi", v_[1].imag());
  jsonBar.insert("Vc", v_[2].real());
  jsonBar.insert("Vci", v_[2].imag());

  // Shunt element power.
  jsonBar.insert("Sha", sh_[0].real());
  jsonBar.insert("Shai", sh_[0].imag());
  jsonBar.insert("Shb", sh_[1].real());
  jsonBar.insert("Shbi", sh_[1].imag());
  jsonBar.insert("Shc", sh_[2].real());
  jsonBar.insert("Shci", sh_[2].imag());

  // Injected Power.
  jsonBar.insert("Sia", si_[0].real());
  jsonBar.insert("Siai", si_[0].imag());
  jsonBar.insert("Sib", si_[1].real());
  jsonBar.insert("Sibi", si_[1].imag());
  jsonBar.insert("Sic", si_[2].real());
  jsonBar.insert("Sici", si_[2].imag());

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
  v_[0].real(jsonBar.value("Va").toDouble());
  v_[0].imag(jsonBar.value("Vai").toDouble());
  v_[1].real(jsonBar.value("Vb").toDouble());
  v_[1].imag(jsonBar.value("Vbi").toDouble());
  v_[2].real(jsonBar.value("Vc").toDouble());
  v_[2].imag(jsonBar.value("Vci").toDouble());

  // Shunt element power.
  sh_[0].real(jsonBar.value("Sha").toDouble());
  sh_[0].imag(jsonBar.value("Shai").toDouble());
  sh_[1].real(jsonBar.value("Shb").toDouble()) ;
  sh_[1].imag(jsonBar.value("Shbi").toDouble());
  sh_[2].real(jsonBar.value("Shc").toDouble());
  sh_[2].imag(jsonBar.value("Shci").toDouble());

  // Injected Power.
  si_[0].real(jsonBar.value("Sia").toDouble());
  si_[0].imag(jsonBar.value("Siai").toDouble());
  si_[1].real(jsonBar.value("Sib").toDouble());
  si_[1].imag(jsonBar.value("Sibi").toDouble());
  si_[2].real(jsonBar.value("Sic").toDouble());
  si_[2].imag(jsonBar.value("Sici").toDouble());

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
  rSi_[1].real(jsonBar.value("rSgb").toDouble());
  rSi_[1].imag(jsonBar.value("rSgbi").toDouble());
  rSi_[2].real(jsonBar.value("rSgc").toDouble());
  rSi_[2].imag(jsonBar.value("rSgci").toDouble());

  // Get Position.
  setX(jsonBar.value("x").toDouble());
  setY(jsonBar.value("y").toDouble());
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF Bar::boundingRect() const
{
  return QRectF(-Network::barIconSize / 2 + network->xOffset,
                -Network::barIconSize / 2 + network->yOffset,
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

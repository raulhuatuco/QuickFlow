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
 * \version 0.3
 * \date 10/2015
 * \copyright David Krepsky
 */

#include "models/bar.h"
#include <QGraphicsScene>
#include <QPainter>
#include "math_constants.h"

/*******************************************************************************
 * Const initialization.
 ******************************************************************************/
const int32_t Bar::kInvalidId = -1;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Bar::Bar()
  : QGraphicsObject(),
    id_(kInvalidId),
    network_(NULL),
    infoBar(NULL),
    barLabel(this)
{
  // Bars can be selected.
  setFlag(ItemIsSelectable);

  // Needed to enable position changing.
  setFlag(ItemSendsGeometryChanges);

  // Will be above lines and under info boxes.
  setZValue(1.0);

  // Add bar label.
  // The 1.3 and 2.0 values were determined experimentaly.
  // They will position the label at the top left corner of the bar.
  barLabel.setX(1.3*Network::barIconSize);
  barLabel.setY(2.0*Network::barIconSize);
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Bar::~Bar()
{
  hideInfo();

  removeLines();

  // Note that we need to check for dangling pointers before removing.
  if(scene() != NULL)
    scene()->removeItem(this);

  if(!network_.isNull())
    network_->removeBar(this);
}

/*******************************************************************************
 * Bar id.
 ******************************************************************************/
int32_t Bar::id()
{
  return id_;
}

/*******************************************************************************
 * Set bar id.
 ******************************************************************************/
void Bar::setBarId(int32_t id)
{
  id_ = id;
  barLabel.setPlainText(QString::number(id_));
}

/*******************************************************************************
 * Network.
 ******************************************************************************/
Network *Bar::network()
{
  return network_;
}

/*******************************************************************************
 * Set network.
 ******************************************************************************/
void Bar::setNetwork(Network *network)
{
  network_ = network;
  adjustLabelPosition();
}

/*******************************************************************************
 * Initial Voltage.
 ******************************************************************************/
complex<double> Bar::v(int8_t phase, Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    return v_[phase]*network_->voltageBase();
    break;

  case Unit::kKiloVolts:
    return v_[phase]*network_->voltageBase()/1000.0;
    break;

  case Unit::kVoltsPerUnit:
    return v_[phase];
    break;

  default:
    return v_[phase];
    break;
  }
}

/*******************************************************************************
 * Set initial Voltage.
 ******************************************************************************/
void Bar::setV(int8_t phase, complex<double> newVoltage,
               Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    v_[phase] = newVoltage/network_->voltageBase();
    break;

  case Unit::kKiloVolts:
    v_[phase] = newVoltage*1000.0/network_->voltageBase();
    break;

  case Unit::kVoltsPerUnit:
    v_[phase] = newVoltage;
    break;

  default:
    v_[phase] = newVoltage;
    break;
  }

  rV_[phase] = v_[phase];
}

/*******************************************************************************
 * Get Shunt Element Power.
 ******************************************************************************/
complex<double> Bar::sh(int8_t phase, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    return sh_[phase]*network_->powerBase();
    break;

  case Unit::kKiloVA:
    return sh_[phase]*network_->powerBase()/1000.0;
    break;

  case Unit::kMegaVa:
    return sh_[phase]*network_->powerBase()/1000000.0;
    break;

  case Unit::kVaPerUnit:
    return sh_[phase];
    break;

  default:
    return sh_[phase];
    break;
  }
}

/*******************************************************************************
 * Set Shunt Element Power.
 ******************************************************************************/
void Bar::setSh(int8_t phase, complex<double> newPower, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    sh_[phase] = newPower/network_->powerBase();
    break;

  case Unit::kKiloVA:
    sh_[phase] = newPower*1000.0/network_->powerBase();
    break;

  case Unit::kMegaVa:
    sh_[phase] = newPower*1000000.0/network_->powerBase();
    break;

  case Unit::kVaPerUnit:
    sh_[phase] = newPower;
    break;

  default:
    sh_[phase] = newPower;
    break;
  }
}

/*******************************************************************************
 * Injected power.
 ******************************************************************************/
complex<double> Bar::si(int8_t phase, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    return si_[phase]*network_->powerBase();
    break;

  case Unit::kKiloVA:
    return si_[phase]*network_->powerBase()/1000.0;
    break;

  case Unit::kMegaVa:
    return si_[phase]*network_->powerBase()/1000000.0;
    break;

  case Unit::kVaPerUnit:
    return si_[phase];
    break;

  default:
    return si_[phase];
    break;
  }
}

/*******************************************************************************
 * Set Injected power.
 ******************************************************************************/
void Bar::setSi(int8_t phase, complex<double> newPower, Unit::PowerUnit unit)
{
  switch (unit) {
  case Unit::kVA:
    si_[phase] = newPower/network_->powerBase();
    break;

  case Unit::kKiloVA:
    si_[phase] = newPower*1000.0/network_->powerBase();
    break;

  case Unit::kMegaVa:
    si_[phase] = newPower*1000000.0/network_->powerBase();
    break;

  case Unit::kVaPerUnit:
    si_[phase] = newPower;
    break;

  default:
    si_[phase] = newPower;
    break;
  }
}

/*******************************************************************************
 * Result voltage.
 ******************************************************************************/
complex<double> Bar::rV(int8_t phase, Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    return rV_[phase]*network_->voltageBase();
    break;

  case Unit::kKiloVolts:
    return rV_[phase]*network_->voltageBase()/1000.0;
    break;

  case Unit::kVoltsPerUnit:
    return rV_[phase];
    break;

  default:
    return rV_[phase];
    break;
  }
}

/*******************************************************************************
 * Set result voltage.
 ******************************************************************************/
void Bar::setRV(int8_t phase, complex<double> resultVoltage,
                Unit::VoltageUnit unit)
{
  switch (unit) {
  case Unit::kVolts:
    rV_[phase] = resultVoltage/network_->voltageBase();
    break;

  case Unit::kKiloVolts:
    rV_[phase] = resultVoltage*1000.0/network_->voltageBase();
    break;

  case Unit::kVoltsPerUnit:
    rV_[phase] = resultVoltage;
    break;

  default:
    rV_[phase] = resultVoltage;
    break;
  }
}

/*******************************************************************************
 * Result value for bar current.
 ******************************************************************************/
complex<double> Bar::rI(int8_t phase, Unit::CurrentUnit unit)
{
  switch (unit) {
  case Unit::kAmpere:
    return rI_[phase]*network_->currentBase();
    break;

  case Unit::kKiloAmpere:
    return rI_[phase]*network_->currentBase()/1000.0;
    break;

  case Unit::kAmperePerUnit:
    return rI_[phase];
    break;

  default:
    return rI_[phase];
    break;
  }
}

/*******************************************************************************
 * Set result value for bar current.
 ******************************************************************************/
void Bar::setRI(int8_t phase, complex<double> resultCurrent,
                Unit::CurrentUnit unit)
{
  switch (unit) {
  case Unit::kAmpere:
    rI_[phase] = resultCurrent/network_->currentBase();
    break;

  case Unit::kKiloAmpere:
    rI_[phase] = resultCurrent*1000.0/network_->currentBase();
    break;

  case Unit::kAmperePerUnit:
    rI_[phase] = resultCurrent;
    break;

  default:
    rI_[phase] = resultCurrent;
    break;
  }
}

/*******************************************************************************
 * Add line.
 ******************************************************************************/
void Bar::addLine(QPointer<Line> line)
{
  // Check for dangling pointer.
  if(!line.isNull())
    lines.append(line);
}

/*******************************************************************************
 * Remove line.
 ******************************************************************************/
void Bar::removeLine(QPointer<Line> line)
{
  if(!line.isNull()) {
    int index = lines.indexOf(line);

    if (index != -1) lines.removeAt(index);
  }
}

/*******************************************************************************
 * Remove lines.
 ******************************************************************************/
void Bar::removeLines()
{
  foreach (QPointer<Line> line, lines) {
    if(!line.isNull())
      delete line;
    else
      lines.remove(lines.indexOf(line));
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
  jsonBar.insert("id", id_);

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
  id_ = jsonBar.value("id").toInt();
  barLabel.setPlainText(QString::number(id_));

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

  // Get Position.
  setX(jsonBar.value("x").toDouble());
  setY(jsonBar.value("y").toDouble());
}

/*******************************************************************************
 * boundingRect.
 ******************************************************************************/
QRectF Bar::boundingRect() const
{
  return QRectF(-Network::barIconSize / 2 + network_->xOffset,
                -Network::barIconSize / 2 + network_->yOffset,
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
      if(line)
        line->updatePosition();
    }

    adjustLabelPosition();
  }
  // if bar is selected, show information box.
  else if (change == ItemSelectedChange) {
    if (value == true) {
      showInfo();
    }
    // Bar has been deselected.
    else {
      hideInfo();
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

  painter->setPen(network_->barContourColor);

  if(id_ > 0)
    drawPq(painter);
  else
    drawSlack(painter);

}

/*******************************************************************************
 * Adjust label position.
 ******************************************************************************/
void Bar::adjustLabelPosition()
{
  // Adjust barLabel position to include the network offset.
  // The 1.3 and 2.0 values were determined experimentaly.
  // They will position the label at the top left corner of the bar.
  double labelX, labelY;
  labelX =  (network_.isNull()) ? 0 : network_->xOffset;
  labelX += -1.3*Network::barIconSize;

  labelY = (network_.isNull()) ? 0 : network_->yOffset;
  labelY += -2.0*Network::barIconSize;

  barLabel.setPos(labelX, labelY);
}

/*******************************************************************************
 * Show info.
 ******************************************************************************/
void Bar::showInfo()
{
  // if multiple items are selected, we don't want to show the info box.
  if(!(scene()->selectedItems().size() >= 1)) {
    // Check if box already exists.
    if (infoBar == NULL) {
      // Create a new box.
      infoBar = new InfoBar(this);
      scene()->addItem(infoBar);
    }
  }
}

/*******************************************************************************
 * Hide info.
 ******************************************************************************/
void Bar::hideInfo()
{
  if (infoBar != NULL) {
    scene()->removeItem(infoBar);
    delete infoBar;
    infoBar = NULL;
  }
}

/*******************************************************************************
 * drawSlack.
 ******************************************************************************/
void Bar::drawSlack(QPainter *painter)
{
  if (isSelected()) {
    painter->setBrush(network_->selectedColor);
  } else {
    painter->setBrush(network_->slackBarColor);
  }

  painter->drawRect(boundingRect());
}

/*******************************************************************************
 * drawPq.
 ******************************************************************************/
void Bar::drawPq(QPainter *painter)
{

  if (isSelected()) {
    painter->setBrush(network_->selectedColor);
  } else {
    painter->setBrush(network_->pqBarColor);
  }

  painter->drawEllipse(boundingRect());
}

/*!
 * \}
 */

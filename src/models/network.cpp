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
 * \file network.cpp
 *
 * \brief Implementation of Network class.
 *
 * This is the implementation of the Network class.
 *
 * \author David Krepsky
 * \version 0.3
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "models/network.h"
#include <QJsonArray>

/*******************************************************************************
 * Constants.
 ******************************************************************************/
// 15.0 seens to be a nice size for bar icons.
const double Network::barIconSize = 15.0;

// 4.0 also looks good.
const double Network::lineWidth = 4.0;

const QColor Network::selectedColor = Qt::red;

/*******************************************************************************
 * Static data initialization.
 ******************************************************************************/
uint32_t Network::maxIterations = 1000;

double Network::minError = 0.01;

Unit::LengthUnit Network::lengthUnit = Unit::kMeter;

Unit::ImpedanceUnit Network::impedanceUnit = Unit::kOhm;

Unit::VoltageUnit Network::voltageUnit = Unit::kVolts;

Unit::PowerUnit Network::powerUnit = Unit::kVA;

Unit::CurrentUnit Network::currentUnit = Unit::kAmpere;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Network::Network() :
  barContourColor(Qt::gray),
  slackBarColor(Qt::green),
  pqBarColor(Qt::blue),
  lineColor(Qt::black)
{
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Network::~Network()
{
  // By deleting all bars, we make sure all lines are also deleted, because bars
  // will altomatically remove all lines they are connected with.
  foreach(QPointer<Bar> bar, bars) {
    // We need to check for dangling pointer because, on program termination,
    // some bars will have problems on deletion.
    if(!bar.isNull())
      delete bar;
  }
}

/*******************************************************************************
 * Add bar.
 ******************************************************************************/
bool Network::addBar(Bar *bar)
{
  if(bar != NULL) {
    // Check if bar exists.
    if(getBarById(bar->id()) != NULL)
      return false;

    QPointer<Bar> newBar(bar);
    bars.insert(bar->id(), newBar);
    bar->setNetwork(this);

    return true;
  }

  return false;
}

/*******************************************************************************
 * Add line.
 ******************************************************************************/
bool Network::addLine(Line *line)
{
  // Check if line exists.
  if(getLineByNodes(line->nodes) != NULL)
    return false;

  // Check if nodes exists.
  Bar *pNoI = getBarById(line->nodes.first);
  Bar *pNoF = getBarById(line->nodes.second);

  if ((pNoI == NULL) || (pNoF == NULL))
    return false;

  line->setNetwork(this);

  line->setNodes(pNoI, pNoF);

  lines.insert(line->nodes, line);

  return true;
}

/*******************************************************************************
 * Remove bar by id.
 ******************************************************************************/
void Network::removeBar(int32_t id)
{
  removeBar(getBarById(id));
}

/*******************************************************************************
 * Remove bar by pointer.
 ******************************************************************************/
void Network::removeBar(Bar *bar)
{
  bars.remove(bar->id());
}

/*******************************************************************************
 * Remove line by nodes.
 ******************************************************************************/
void Network::removeLine(QPair<int32_t, int32_t> nodes)
{
  removeLine(getLineByNodes(nodes));
}

/*******************************************************************************
 * Remove line by pointers.
 ******************************************************************************/
void Network::removeLine(Line *line)
{
  lines.remove(line->nodes);
}

/*******************************************************************************
 * Get bar by id.
 ******************************************************************************/
Bar *Network::getBarById(uint32_t id)
{
  return bars.value(id, NULL);
}

/*******************************************************************************
 * Get line by nodes.
 ******************************************************************************/
Line *Network::getLineByNodes(QPair<int32_t, int32_t> nodes)
{
  return lines.value(nodes, NULL);
}

/*******************************************************************************
 * Network to Json.
 ******************************************************************************/
QJsonObject Network::toJson()
{
  QJsonObject netJson;

  netJson.insert("name", name);
  netJson.insert("xOffset", xOffset);
  netJson.insert("yOffset", yOffset);
  netJson.insert("barStrokeColor", barContourColor.name());
  netJson.insert("barSlackFillColor", slackBarColor.name());
  netJson.insert("barPqFillColor", pqBarColor.name());
  netJson.insert("lineColor", lineColor.name());
  netJson.insert("voltageBase", voltageBase_);
  netJson.insert("powerBase", powerBase_);

  QJsonArray barArray;
  foreach(Bar *bar,bars) {
    barArray << bar->toJson();
  }
  netJson.insert("barArray", barArray);

  QJsonArray lineArray;
  foreach(Line *line, lines) {
    lineArray << line->toJson();
  }
  netJson.insert("lineArray", lineArray);

  return netJson;
}

/*******************************************************************************
 *  Network from Json.
 ******************************************************************************/
void Network::fromJson(QJsonObject &netJson)
{
  name = netJson.value("name").toString();
  xOffset = netJson.value("xOffset").toDouble();
  yOffset = netJson.value("yOffset").toDouble();
  barContourColor.setNamedColor(netJson.value("barStrokeColor").toString());
  slackBarColor.setNamedColor(netJson.value("barSlackFillColor").toString());
  pqBarColor.setNamedColor(netJson.value("barPqFillColor").toString());
  lineColor.setNamedColor(netJson.value("lineColor").toString());
  voltageBase_ = netJson.value("voltageBase").toDouble();
  powerBase_ = netJson.value("powerBase").toDouble();

  QJsonArray barArray(netJson.value("barArray").toArray());

  foreach(QJsonValue barValue, barArray) {
    QJsonObject jsonBar(barValue.toObject());

    Bar *bar = new Bar;
    bar->fromJson(jsonBar);
    addBar(bar);
  }

  QJsonArray lineArray;
  lineArray = netJson.value("lineArray").toArray();

  foreach(QJsonValue lineValue, lineArray) {
    QJsonObject jsonLine(lineValue.toObject());

    Line *line = new Line;
    line->fromJson(jsonLine);
    addLine(line);
  }
}

/*******************************************************************************
 * Current base.
 ******************************************************************************/
double Network::currentBase()
{
  return powerBase_/voltageBase_;
}

/*******************************************************************************
 * Impedance base.
 ******************************************************************************/
double Network::impedanceBase()
{
  return voltageBase_*(voltageBase_/powerBase_);
}

/*******************************************************************************
 * Voltage base.
 ******************************************************************************/
double Network::voltageBase()
{
  return voltageBase_;
}

/*******************************************************************************
 * Power base.
 ******************************************************************************/
double Network::powerBase()
{
  return powerBase_;
}

/*******************************************************************************
 * Set voltage base.
 ******************************************************************************/
void Network::setVoltageBase(double voltage)
{
  voltageBase_ = voltage;
}

/*******************************************************************************
 * Set power base.
 ******************************************************************************/
void Network::setPowerBase(double power)
{
  powerBase_ = power;
}

/*!
 * \}
 */

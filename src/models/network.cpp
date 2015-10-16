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
 * \addtogroup Graphics
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
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#include <QJsonArray>
#include "models/network.h"

/*******************************************************************************
 * Const.
 ******************************************************************************/
const double Network::barIconSize = 15.0;
const double Network::lineWidth = 4.0;
const QColor Network::selectedColor = Qt::red;

/*******************************************************************************
 * Static data initialization.
 ******************************************************************************/
u_int32_t Network::maxIterations = 1000;

double Network::minError = 0.01;

double Network::voltageBase = 23000.0;

double Network::powerBase = 2500000.0;

Unit::LengthUnit Network::lengthUnit = Unit::kMeter;

Unit::ImpedanceUnit Network::impedanceUnit = Unit::kOhm;

Unit::VoltageUnit Network::voltageUnit = Unit::kVolts;

Unit::PowerUnit Network::powerUnit = Unit::kVA;

Unit::CurrentUnit Network::currentUnit = Unit::kAmpere;

/*******************************************************************************
 * Current Base.
 ******************************************************************************/
double Network::currentBase()
{
  return powerBase/(sqrt(3)*voltageBase);
}

/*******************************************************************************
 * Impedance Base.
 ******************************************************************************/
double Network::impedanceBase()
{
  return voltageBase*voltageBase/powerBase;
}

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Network::Network() :
  barStrokeColor(Qt::gray),
  barSlackFillColor(Qt::green),
  barPqFillColor(Qt::blue),
  lineColor(Qt::black)
{
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Network::~Network()
{
  foreach(Bar *bar, bars) {
    delete bar;
  }

  foreach(Line *line, lines) {
    delete line;
  }
}

/*******************************************************************************
 * addBar.
 ******************************************************************************/
bool Network::addBar(Bar *bar)
{
  // Check if bar exists.
  if(getBarById(bar->id) != NULL)
    return false;

  bars.insert(bar->id, bar);
  bar->network = this;

  return true;
}

/*******************************************************************************
 * addLine.
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

  line->network = this;

  line->setNodes(pNoI, pNoF);

  lines.insert(line->nodes, line);

  return true;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeBar(int32_t id)
{
  removeBar(getBarById(id));
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeBar(Bar *bar)
{
  delete bar;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeLine(QPair<int32_t, int32_t> nodes)
{
  removeLine(getLineByNodes(nodes));
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeLine(Line *line)
{
  delete line;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
Bar *Network::getBarById(u_int32_t id)
{
  return bars.value(id, NULL);
}

/*******************************************************************************
 * getLineByNodes.
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
  netJson.insert("barStrokeColor", barStrokeColor.name());
  netJson.insert("barSlackFillColor", barSlackFillColor.name());
  netJson.insert("barPqFillColor", barPqFillColor.name());
  netJson.insert("lineColor", lineColor.name());

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
  barStrokeColor.setNamedColor(netJson.value("barStrokeColor").toString());
  barSlackFillColor.setNamedColor(netJson.value("barSlackFillColor").toString());
  barPqFillColor.setNamedColor(netJson.value("barPqFillColor").toString());
  lineColor.setNamedColor(netJson.value("lineColor").toString());

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

/*!
 * \}
 */

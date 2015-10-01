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
#include "graphics/network.h"

/*******************************************************************************
 * Static Variables Initialization.
 ******************************************************************************/
double Network::barIconSize = 15.0;
double Network::lineWidth = 4.0;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
Network::Network() :
  QGraphicsScene(),
  selectedColor(Qt::red),
  barStrokeColor(Qt::gray),
  barSlackFillColor(Qt::green),
  barPqFillColor(Qt::blue),
  lineColor(Qt::black),
  maxIterations(1000),
  minError(0.001),
  voltageBase(0.0),
  powerBase(0.0),
  lengthUnit(Unit::kMeter),
  impedanceUnit(Unit::kOhm),
  voltageUnit(Unit::kKiloVolts),
  powerUnit(Unit::kKiloVA)
{

  barDoubleClick = new QSignalMapper(this);
  lineDoubleClick = new QSignalMapper(this);

  // Create mapper for bar properties.
  connect(barDoubleClick, SIGNAL(mapped(QObject *)), this,
          SIGNAL(barProperties(QObject *)));

  // Create mapper for line properties.
  connect(lineDoubleClick, SIGNAL(mapped(QObject *)), this,
          SIGNAL(lineProperties(QObject *)));
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
Network::~Network()
{

}

/*******************************************************************************
 * addBar.
 ******************************************************************************/
bool Network::addBar(Bar *bar)
{
  // Check if bar exists.
  if(getBarById(bar->id) != NULL)
    return false;

  // Add bar to this group.
  addItem(bar);

  bars.insert(bar->id, bar);

  // Connect signals.
  connect(bar, SIGNAL(eventDoubleClick()), barDoubleClick, SLOT(map()));
  barDoubleClick->setMapping(bar, bar);

  return true;
}

/*******************************************************************************
 * addLine.
 ******************************************************************************/
bool Network::addLine(Line *line)
{
  // Check if line exists.
  if(getLineByNodes(line->noI, line->noF) != NULL)
    return false;

  // Check if nodes exists.
  Bar *pNoI = getBarById(line->noI);
  Bar *pNoF = getBarById(line->noF);

  if ((pNoI == NULL) || (pNoF == NULL))
    return false;

  line->setNodes(pNoI, pNoF);

  // Add line to this group.
  addItem(line);
  lines.append(line);

  // Connect signals.
  connect(line, SIGNAL(eventDoubleClick()), lineDoubleClick, SLOT(map()));
  lineDoubleClick->setMapping(line, line);

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
  removeItem(bar);
  delete bar;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeLine(int32_t noI, int32_t noF)
{
  removeLine(getLineByNodes(noI, noF));
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
void Network::removeLine(Line *line)
{
  removeItem(line);
  delete line;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
Bar *Network::getBarById(uint32_t id)
{
  return bars.value(id, NULL);
}

/*******************************************************************************
 * getLineByNodes.
 ******************************************************************************/
Line *Network::getLineByNodes(int32_t noI, int32_t noF)
{
  foreach (Line *line, lines) {
    if ((line->noI == noI) && (line->noF == noF))
      return line;

    if ((line->noF == noI) && (line->noI == noF))
      return line;
  }

  return NULL;
}

/*!
 * \}
 */

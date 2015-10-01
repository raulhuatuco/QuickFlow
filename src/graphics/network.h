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
 * \file network.h
 *
 * \brief Network class definition.
 *
 * This file defines the Network class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#ifndef GRAPHICS_NETWORK_H
#define GRAPHICS_NETWORK_H

#include <QGraphicsScene>
#include <QSignalMapper>
#include <QObject>
#include <QVector>
#include <QMap>
#include "customtypes.h"
#include "models/bar.h"
#include "models/line.h"

QT_BEGIN_NAMESPACE
class Bar;
class Line;
QT_END_NAMESPACE

/*!
 * \class Network
 * \brief Represents a power distribution systems.
 *
 * ### Overview
 * Network class represents a power distribution system, providing the data
 * structure and methods that models a real system.
 * The class provides graphical.
 *
 * ### Example
 * \code
 *
 * \endcode
 */
class Network : public QGraphicsScene
{
  Q_OBJECT

public:
  /*!
   * \brief barIconSize
   */
  static double barIconSize;

  /*!
   * \brief lineWidth
   */
  static double lineWidth;

  /*!
   * \brief selectedColor
   */
  QColor selectedColor;

  /*!
   * \brief barStrokeColor
   */
  QColor barStrokeColor;

  /*!
   * \brief barSlackFillColor
   */
  QColor barSlackFillColor;

  /*!
   * \brief barPqFillColor
   */
  QColor barPqFillColor;

  /*!
   * \brief lineColor
   */
  QColor lineColor;

  /*!
   * \brief maxIterations
   */
  uint32_t maxIterations;

  /*!
   * \brief minError
   */
  double minError;

  /*!
   * \brief voltageBase
   */
  double voltageBase;

  /*!
   * \brief powerBase
   */
  double powerBase;

  /*!
   * \brief lengthUnit
   */
  Unit::LengthUnit lengthUnit;

  /*!
   * \brief impedanceUnit
   */
  Unit::ImpedanceUnit impedanceUnit;

  /*!
   * \brief voltageUnit
   */
  Unit::VoltageUnit voltageUnit;

  /*!
   * \brief powerUnit
   */
  Unit::PowerUnit powerUnit;

  /*!
   * \brief bars
   */
  QMap<int32_t, Bar *> bars;

  /*!
   * \brief lines
   */
  QVector<Line *> lines;

  /*!
   * \brief Network
   */
  Network();

  /*!
   * \brief
   *
   */
  ~Network();

  /*!
   * \brief addBar
   * \param bar
   * \return
   */
  bool addBar(Bar *bar);

  /*!
   * \brief addLine
   * \param line
   * \return
   */
  bool addLine(Line *line);

  /*!
   * \brief removeBar
   * \param id
   */
  void removeBar(int32_t id);

  /*!
   * \brief removeBar
   * \param bar
   */
  void removeBar(Bar *bar);

  /*!
   * \brief removeLine
   * \param noI
   * \param noF
   */
  void removeLine(int32_t noI, int32_t noF);

  /*!
   * \brief removeLine
   * \param noI
   * \param noF
   */
  void removeLine(Line *line);

  /*!
   * \brief getBarById
   * \param id
   * \return
   */
  Bar *getBarById(uint32_t id);

  /*!
   * \brief getLineByNodes
   * \param noI
   * \param noF
   * \return
   */
  Line *getLineByNodes(int32_t noI, int32_t noF);

signals:
  /*!
   * \brief barProperties
   */
  void barProperties(QObject *);

  /*!
   * \brief lineProperties
   */
  void lineProperties(QObject *);

private:
             
  /*!
   * \brief barDoubleClick
   */
  QSignalMapper *barDoubleClick;

  /*!
   * \brief lineDoubleClick
   */
  QSignalMapper *lineDoubleClick;

};

#endif  // GRAPHICS_NETWORK_H

/*!
 * \}
 */

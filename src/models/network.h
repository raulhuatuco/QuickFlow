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
 * \file network.h
 *
 * \brief Network class definition.
 *
 * This file defines the Network class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_NETWORK_H
#define MODELS_NETWORK_H

#include <QGraphicsScene>
#include <QObject>
#include <QVector>
#include <QJsonObject>
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
 *
 * ### Example
 * \code
 *
 * \endcode
 */
class Network : public QObject
{
  Q_OBJECT

public:

  /*****************************************************************************
   * Const.
   ****************************************************************************/
  static const double barIconSize;

  static const double lineWidth;

  static const QColor selectedColor;

  /*****************************************************************************
   * Static data.
   ****************************************************************************/
  static u_int32_t maxIterations;

  static double minError;

  static double voltageBase;

  static double powerBase;

  static Unit::LengthUnit lengthUnit;

  static Unit::ImpedanceUnit impedanceUnit;

  static Unit::VoltageUnit voltageUnit;

  static Unit::PowerUnit powerUnit;

  static Unit::CurrentUnit currentUnit;

  /*****************************************************************************
   * Properties.
   ****************************************************************************/
  QString name;

  double xOffset;

  double yOffset;

  QColor barStrokeColor;

  QColor barSlackFillColor;

  QColor barPqFillColor;

  QColor lineColor;

  QMap<int32_t, Bar *> bars;

  QMap<QPair<int32_t, int32_t>, Line *> lines;

  /*****************************************************************************
   * Class functions.
   ****************************************************************************/
  static double currentBase();

  static double impedanceBase();

  /*****************************************************************************
   * Constructor.
   ****************************************************************************/
  Network();

  /*****************************************************************************
   * Destructor.
   ****************************************************************************/
  ~Network();

  /*****************************************************************************
   * Add bar.
   ****************************************************************************/
  bool addBar(Bar *bar);

  /*****************************************************************************
   * Add line.
   ****************************************************************************/
  bool addLine(Line *line);

  /*****************************************************************************
   * Remove bar by id.
   ****************************************************************************/
  void removeBar(int32_t id);

  /*****************************************************************************
   * Remove bar by pointer.
   ****************************************************************************/
  void removeBar(Bar *bar);

  /*****************************************************************************
   * Remove line by nodes.
   ****************************************************************************/
  void removeLine(QPair<int32_t, int32_t> nodes);

  /*****************************************************************************
   * Remove line by pointer.
   ****************************************************************************/
  void removeLine(Line *line);

  /*****************************************************************************
   * Get bar by id.
   ****************************************************************************/
  Bar *getBarById(u_int32_t id);

  /*****************************************************************************
   * Get line by nodes.
   ****************************************************************************/
  Line *getLineByNodes(QPair<int32_t, int32_t> nodes);

  /*****************************************************************************
   * Network to Json.
   ****************************************************************************/
  QJsonObject toJson();

  /*****************************************************************************
   * Network from Json.
   ****************************************************************************/
  void fromJson(QJsonObject &netJson);
};

#endif  // MODELS_NETWORK_H

/*!
 * \}
 */

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
 * \file line.h
 *
 * \brief line class definition.
 *
 * This file defines the line class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_LINE_H
#define MODELS_LINE_H

#include <QJsonObject>
#include <QGraphicsObject>
#include <complex>
#include "models/bar.h"
#include "models/network.h"
#include "graphics/infoline.h"

QT_BEGIN_NAMESPACE
class Bar;
class InfoLine;
class Network;
QT_END_NAMESPACE

using std::complex;

/*!
 * \class Line
 * \brief Represents a cable in power distribution systems.
 *
 * ### Overview
 * Line class represents a line in a power system, providing the data structure
 * and methods that model a real line.
 *
 * ### Example
 * \code
 *
 * \endcode
 */
class Line : public QGraphicsObject
{
  Q_OBJECT

public:
  /*****************************************************************************
   * Constants.
   ****************************************************************************/
  static const int32_t kInvalidNode;

  /*****************************************************************************
   * Properties.
   ****************************************************************************/
  QPair<int32_t, int32_t> nodes;

  Network *network;

  /*****************************************************************************
   * Constructor.
   ****************************************************************************/
  Line();

  /*****************************************************************************
   * Destructor.
   ****************************************************************************/
  ~Line();

  /*****************************************************************************
   * Impedance.
   ****************************************************************************/
  complex<double>  z(int32_t index, Unit::ImpedanceUnit unit = Unit::kOhm);

  /*****************************************************************************
   * Set impedance.
   ****************************************************************************/
  void setZ(int32_t index, complex<double> newImpedance,
            Unit::ImpedanceUnit unit = Unit::kOhm);

  /*****************************************************************************
   * Impedance in per unit.
   ****************************************************************************/
  complex<double> zPu(int32_t index);

  /*****************************************************************************
   * Current.
   ****************************************************************************/
  complex<double> i(int32_t phase, Unit::CurrentUnit unit = Unit::kAmpere);

  /*****************************************************************************
   * Set current.
   ****************************************************************************/
  void setI(int32_t phase, complex<double> newCurrent,
            Unit::CurrentUnit unit = Unit::kAmpere);

  /*****************************************************************************
   * Current in per unit.
   ****************************************************************************/
  complex<double> iPu(int32_t phase);

  /*****************************************************************************
   * Compute the loss in the line.
   ****************************************************************************/
  complex<double> loss(int32_t phase, Unit::PowerUnit);

  /*****************************************************************************
   * Line length.
   ****************************************************************************/
  double length(Unit::LengthUnit unit = Unit::kMeter);

  /*****************************************************************************
  * Set line length.
  ****************************************************************************/
  void setLength(double newLength, Unit::LengthUnit unit = Unit::kMeter);

  /*****************************************************************************
   * Initial node pointer.
   ****************************************************************************/
  Bar *pNoI();

  /*****************************************************************************
   * Final node pointer.
   ****************************************************************************/
  Bar *pNoF();

  /*****************************************************************************
   * Set initial and final nodes.
   ****************************************************************************/
  void setNodes(Bar *pNoI, Bar *pNoF);

  /*****************************************************************************
   * Convert line data to Json object.
   ****************************************************************************/
  QJsonObject toJson();

  /*****************************************************************************
   * Convert Json object to line.
   ****************************************************************************/
  void fromJson(QJsonObject &lineJson);

  /*****************************************************************************
   * Update line position.
   ****************************************************************************/
  void updatePosition();

  /*****************************************************************************
   * Line bounding rect.
   ****************************************************************************/
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  /*****************************************************************************
   * Line Shape.
   ****************************************************************************/
  QPainterPath shape() const Q_DECL_OVERRIDE;

  /*****************************************************************************
   * Item Change.
   ****************************************************************************/
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;

signals:
  /*****************************************************************************
   * Double Click Event.
   ****************************************************************************/
  /*!
   * \brief eventDoubleClick
   */
  void eventDoubleClick();

protected:
  /*****************************************************************************
   * Double click event handler.
   ****************************************************************************/
  /*!
   * \brief mouseDoubleClickEvent
   * \param event
   */
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

  /*****************************************************************************
   * Paint.
   ****************************************************************************/
  /*!
   * \brief
   *
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  /*****************************************************************************
   * Private properties.
   ****************************************************************************/
  double length_;

  /*!
   * \brief pNoI_
   */
  Bar *pNoI_;

  /*!
   * \brief pNoF_
   */
  Bar *pNoF_;

  complex<double> z_[6];
  complex<double> i_[3];

  QLineF coords;

  /*!
   * \brief selectionArea
   */
  QPolygonF selectionArea;

  /*!
   * \brief infoLine
   */
  InfoLine *infoLine;
};

#endif  // MODELS_LINE_H

/*!
 * \}
 */

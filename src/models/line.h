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
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_LINE_H
#define MODELS_LINE_H

#include <QJsonObject>
#include <QGraphicsObject>
#include <complex>
#include "models/bar.h"
#include "graphics/infoline.h"
#include "graphics/network.h"

QT_BEGIN_NAMESPACE
class Bar;
class InfoLine;
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
  /*!
   * \brief kInvalidNode
   */
  static const int32_t kInvalidNode = -1;

  /*!
   * \brief noI
   */
  int32_t noI;

  /*!
   * \brief noF
   */
  int32_t noF;

  // Length
  /*!
   * \brief length
   */
  double length;

  /*!
     * \brief Line
     */
  Line();

  /*!
   * \brief
   *
   */
  ~Line();

  // Parent bars pointer.
  /*!
   * \brief pNoI
   * \return
   */
  Bar *pNoI();

  /*!
   * \brief pNoF
   * \return
   */
  Bar *pNoF();

  /*!
   * \brief setNodes
   * \param pNoI
   * \param pNoF
   */
  void setNodes(Bar *pNoI, Bar *pNoF);

  complex<double> Zaa;
  complex<double> Zab;
  complex<double> Zac;
  complex<double> Zbb;
  complex<double> Zbc;
  complex<double> Zcc;

  complex<double> Zaa_pu(double impedanceBase);
  complex<double> Zab_pu(double impedanceBase);
  complex<double> Zac_pu(double impedanceBase);
  complex<double> Zbb_pu(double impedanceBase);
  complex<double> Zbc_pu(double impedanceBase);
  complex<double> Zcc_pu(double impedanceBase);

  // Current
  complex<double> Ia;
  complex<double> Ib;
  complex<double> Ic;
  complex<double> In;

  // Loss
  complex<double> lossA();
  complex<double> lossB();
  complex<double> lossC();

  // Cast.
  /*!
   * \brief toJson
   * \return
   */
  QJsonObject toJson();

  /*!
   * \brief fromJson
   * \param lineJson
   */
  void fromJson(QJsonObject &lineJson);

  /*!
   * \brief updatePosition
   */
  void updatePosition();

  /*!
   * \brief
   *
   * \return
   */
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   * \return
   */
  QPainterPath shape() const Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   * \return
   */
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;

signals:
  /*!
   * \brief eventDoubleClick
   */
  void eventDoubleClick();

protected:
  /*!
   * \brief mouseDoubleClickEvent
   * \param event
   */
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  /*!
   * \brief pNoI_
   */
  Bar *pNoI_;

  /*!
   * \brief pNoF_
   */
  Bar *pNoF_;

  /*!
   * \brief coords
   */
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

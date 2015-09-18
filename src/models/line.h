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
#include <armadillo>
#include "models/bar.h"
#include "graphics/infoline.h"
#include "graphics/network.h"

QT_BEGIN_NAMESPACE
class Bar;
class InfoLine;
QT_END_NAMESPACE

using arma::Mat;
using arma::cx_double;

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
   * \brief lineWidth
   */
  static double lineWidth;

  /*!
   * \brief lineColor
   */
  static QColor lineColor;

  /*!
   * \brief selectedLineColor
   */
  static QColor selectedLineColor;

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

  // Impedance
  /*!
   * \brief Z
   */
  Mat<cx_double> &Z() const;

  /*!
   * \brief setZaa
   * \param Zaa
   */
  void setZaa(cx_double Zaa);

  /*!
   * \brief setZab
   * \param Zab
   */
  void setZab(cx_double Zab);

  /*!
   * \brief setZac
   * \param Zac
   */
  void setZac(cx_double Zac);

  /*!
   * \brief setZbb
   * \param Zbb
   */
  void setZbb(cx_double Zbb);

  /*!
   * \brief setZbc
   * \param Zbc
   */
  void setZbc(cx_double Zbc);

  /*!
   * \brief setZcc
   * \param Zcc
   */
  void setZcc(cx_double Zcc);

  /*!
   * \brief Zpu
   * \return
   */
  Mat<cx_double> Z_pu(double impedanceBase) const;

  // Current
  /*!
   * \brief Ia
   * \return
   */
  Col<cx_double> &I() const;

  /*!
   * \brief setI
   * \param Ia
   * \param Ib
   * \param Ic
   */
  void setI(cx_double &Ia, cx_double &Ib, cx_double &Ic);

  // Loss
  Col<cx_double> lineLoss();

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
   * \brief Z_
   */
  Mat<cx_double> Z_;

  /*!
   * \brief I_
   */
  Col<cx_double> I_;

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

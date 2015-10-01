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
 * \file bar.h
 *
 * \brief Bar class definition.
 *
 * This file defines the Bar class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#ifndef MODELS_BAR_H
#define MODELS_BAR_H

#include <QJsonObject>
#include <QGraphicsObject>
#include <armadillo>
#include <complex>
#include <stdint.h>
#include "models/line.h"
#include "graphics/network.h"
#include "graphics/infobar.h"

QT_BEGIN_NAMESPACE
class Line;
class Network;
class InfoBar;
QT_END_NAMESPACE

using arma::cx_double;

/*!
 * \class Bar
 * \brief Represents a busbar in power distribution systems.
 *
 * ### Overview
 * Bar class represents a bar in a power system, providing the data structure
 * and methods that model a real bar.
 * A bar with id equals to 0 is an alimentator (Slack bar).
 * The class provides graphical.
 *
 * ### Example
 * \code
 *
 * \endcode
 */
class Bar : public QGraphicsObject
{
  Q_OBJECT

public:

  static constexpr int32_t kInvalidId = -1;

  int32_t id;

  QVector<Line *> lines;

  Bar();

  Bar(cx_double initialV, cx_double initialSh = 0.0,
      cx_double initialSi = 0.0);

  virtual ~Bar();

  // Input parameters.
  cx_double Va;
  cx_double Vb;
  cx_double Vc;

  cx_double Va_pu(double voltageBase);
  cx_double Vb_pu(double voltageBase);
  cx_double Vc_pu(double voltageBase);

  cx_double Sha;
  cx_double Shb;
  cx_double Shc;
  cx_double Sha_pu(double powerBase);
  cx_double Shb_pu(double powerBase);
  cx_double Shc_pu(double powerBase);

  cx_double Sia;
  cx_double Sib;
  cx_double Sic;
  cx_double Sia_pu(double powerBase);
  cx_double Sib_pu(double powerBase);
  cx_double Sic_pu(double powerBase);

  cx_double rVa;
  cx_double rVb;
  cx_double rVc;
  cx_double rVa_pu(double powerBase);
  cx_double rVb_pu(double powerBase);
  cx_double rVc_pu(double powerBase);

  cx_double rSia;
  cx_double rSib;
  cx_double rSic;
  cx_double rSia_pu(double powerBase);
  cx_double rSib_pu(double powerBase);
  cx_double rSic_pu(double powerBase);


  // Line manipulation.
  /*!
   * \brief addLine
   * \param line
   */
  void addLine(Line *line);

  /*!
   * \brief removeLine
   * \param line
   */
  void removeLine(Line *line);

  /*!
   * \brief removeLines
   */
  void removeLines();

  // Conversion.
  /*!
   * \brief toJson
   * \return
   */
  QJsonObject toJson();

  /*!
   * \brief fromJson
   * \param jsonBar
   */
  void fromJson(QJsonObject &jsonBar);

  // Graphics.
  /*!
   * \brief
   *
   * \return
   */
  QRectF boundingRect() const Q_DECL_OVERRIDE;

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
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

  /*!
   * \brief
   *
   * \return
   */
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  /*!
   * \brief infoBar
   */
  InfoBar *infoBar;

  /*!
   * \brief drawSlack
   * \param painter
   */
  void drawSlack(QPainter *painter);

  /*!
   * \brief drawPq
   * \param painter
   */
  void drawPq(QPainter *painter);
};

#endif  // MODELS_BAR_H

/*!
 * \}
 */

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
 * \file SystemView.h
 *
 * \brief SystemView class definition.
 *
 * This file defines the SystemView class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#ifndef GRAPHICS_SYSTEMVIEW_H
#define GRAPHICS_SYSTEMVIEW_H

#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include "models/network.h"

/*!
 * \class SystemView
 * \brief Widget to visualise Networks.
 *
 * ### Overview
 * SystemView.
 *
 * ### Example
 * \code
 *
 * \endcode
 */
class SystemView : public QGraphicsView
{
  Q_OBJECT

public:
  /*!
   * \brief SystemView
   * \param parent
   */
  SystemView(QWidget *parent);

  /*!
   * \brief
   *
   */
  ~SystemView();

  /*!
   * \brief zoomIn
   */
  void zoomIn();

  /*!
   * \brief zoomOut
   */
  void zoomOut();

  /*!
   * \brief zoomFit
   */
  void zoomFit();

  /*!
   * \brief addNetwork
   * \param network
   */
  void addNetwork(Network *network);

  /*!
   * \brief removeNetwork
   * \param network
   */
  void removeNetwork(Network *network);

  void addBar(Bar *bar);

  void removeBar(Bar *bar);

  void addLine(Line *line);

  void removeLine(Line *line);

signals:

  void barProperties(QObject *);

  void lineProperties(QObject *);

protected:
  /*!
   * \brief
   *
   */
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   */
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   */
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

  /*!
   * \brief
   *
   */
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  
private:
  /*!
   * \brief kZoomStep
   */
  static const qreal kZoomStep;

  /*!
   * \brief oldPos
   */
  QPoint oldPos;

  QSignalMapper *barDoubleClick;

  QSignalMapper *lineDoubleClick;
};

#endif  // GRAPHICS_SYSTEMVIEW_H

/*!
 * \}
 */

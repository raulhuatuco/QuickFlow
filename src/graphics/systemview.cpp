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
 * \file SystemView.cpp
 *
 * \brief Implementation of SystemView class.
 *
 * This is the implementation of the SystemView class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 09/2015
 * \copyright David Krepsky
 */

#include "window/barproperties.h"
#include "window/lineproperties.h"

#include "graphics/systemview.h"
#include <QColor>
#include <QBrush>

const qreal SystemView::kZoomStep = 0.05;

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
SystemView::SystemView(QWidget *parent) :
  QGraphicsView(parent)
{
  setDragMode(RubberBandDrag);
  // Enable antialiasing
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  setScene(new QGraphicsScene(this));

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
SystemView::~SystemView()
{

}

/*******************************************************************************
 * zoomIn.
 ******************************************************************************/
void SystemView::zoomIn()
{
  scale(1.0 + kZoomStep, 1.0 + kZoomStep);
}

/*******************************************************************************
 * zoomOut.
 ******************************************************************************/
void SystemView::zoomOut()
{
  scale(1.0 - kZoomStep, 1.0 - kZoomStep);
}

/*******************************************************************************
 * zoomFit.
 ******************************************************************************/
void SystemView::zoomFit()
{
  fitInView(sceneRect(), Qt::KeepAspectRatio);
}

/*******************************************************************************
 * addNetwork.
 ******************************************************************************/
void SystemView::addNetwork(Network *network)
{
  foreach (Bar *bar, network->bars) {
    addBar(bar);
  }

  foreach (Line *line, network->lines) {
    addLine(line);
  }
}

/*******************************************************************************
 * removeNetwork.
 ******************************************************************************/
void SystemView::removeNetwork(Network *network)
{

  foreach (Bar *bar, network->bars) {
    removeBar(bar);
  }

  foreach (Line *line, network->lines) {
    removeLine(line);
  }
}

void SystemView::addBar(Bar *bar)
{
  scene()->addItem(bar);

  // Connect signals.
  connect(bar, SIGNAL(eventDoubleClick()), barDoubleClick, SLOT(map()));
  barDoubleClick->setMapping(bar, bar);
}

void SystemView::removeBar(Bar *bar)
{
  scene()->removeItem(bar);

  disconnect(bar, SIGNAL(eventDoubleClick()), barDoubleClick, SLOT(map()));
  barDoubleClick->removeMappings(bar);
}

void SystemView::addLine(Line *line)
{
  scene()->addItem(line);

  // Connect signals.
  connect(line, SIGNAL(eventDoubleClick()), lineDoubleClick, SLOT(map()));
  lineDoubleClick->setMapping(line, line);
}

void SystemView::removeLine(Line *line)
{
  scene()->removeItem(line);
  disconnect(line, SIGNAL(eventDoubleClick()), lineDoubleClick, SLOT(map()));
  lineDoubleClick->removeMappings(line);
}

/*******************************************************************************
 * wheelEvent.
 ******************************************************************************/
void SystemView::wheelEvent(QWheelEvent *event)
{
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  if (event->delta() > 0) {
    scale(1.0 + kZoomStep, 1.0 + kZoomStep);
  } else {
    scale(1.0 - kZoomStep, 1.0 - kZoomStep);
  }
}

/*******************************************************************************
 * mouseMoveEvent.
 ******************************************************************************/
void SystemView::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons().testFlag(Qt::MidButton)) {
    QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    QPoint delta = event->pos() - oldPos;
    oldPos = event->pos();
    hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
    vBar->setValue(vBar->value() - delta.y());
  }

  QGraphicsView::mouseMoveEvent(event);
}

/*******************************************************************************
 * mousePressEvent.
 ******************************************************************************/
void SystemView::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::MidButton) {
    oldPos = event->pos();
    setDragMode(NoDrag);
    setCursor(Qt::OpenHandCursor);
  }

  QGraphicsView::mousePressEvent(event);
}

/*******************************************************************************
 * mouseReleaseEvent.
 ******************************************************************************/
void SystemView::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button() == Qt::MidButton) {
    setDragMode(RubberBandDrag);
    setCursor(Qt::ArrowCursor);
  }

  QGraphicsView::mouseReleaseEvent(event);
}


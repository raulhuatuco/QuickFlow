#include "view.h"

#include <QColor>
#include <QBrush>

/*******************************************************************************
 * PnView.
 ******************************************************************************/
PnView::PnView(QWidget *parent) : QGraphicsView(parent), pnNetwork_(NULL)
{
  setDragMode(RubberBandDrag);
  // Enable antialiasing
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

/*******************************************************************************
 * ~PnView.
 ******************************************************************************/
PnView::~PnView() {}

/*******************************************************************************
 * zoomIn.
 ******************************************************************************/
void PnView::zoomIn()
{
  scale(1.0 + kZoomStep, 1.0 + kZoomStep);
}

/*******************************************************************************
 * zoomOut.
 ******************************************************************************/
void PnView::zoomOut()
{
  scale(1.0 - kZoomStep, 1.0 - kZoomStep);
}

/*******************************************************************************
 * zoomFit.
 ******************************************************************************/
void PnView::zoomFit()
{
  fitInView(sceneRect(), Qt::KeepAspectRatio);
}

/*******************************************************************************
 * setPnNetwork.
 ******************************************************************************/
void PnView::setPnNetwork(Network *pnNetwork)
{
  pnNetwork_ = pnNetwork;
  setScene(pnNetwork_);
}

/*******************************************************************************
 * getPnNetwork.
 ******************************************************************************/
Network *PnView::getPnNetwork()
{
  return pnNetwork_;
}

/*******************************************************************************
 * wheelEvent.
 ******************************************************************************/
void PnView::wheelEvent(QWheelEvent *event)
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
void PnView::mouseMoveEvent(QMouseEvent *event)
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
void PnView::mousePressEvent(QMouseEvent *event)
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
void PnView::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button() == Qt::MidButton) {
    setDragMode(RubberBandDrag);
    setCursor(Qt::ArrowCursor);
  }

  QGraphicsView::mouseReleaseEvent(event);
}

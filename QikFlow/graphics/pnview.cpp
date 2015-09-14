#include "pnview.h"

#include <QColor>
#include <QBrush>

/*******************************************************************************
 * PnView.
 ******************************************************************************/
PnView::PnView(QWidget *parent) : QGraphicsView(parent), pnNetwork_(NULL)
{
  setDragMode(RubberBandDrag);
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
void PnView::setPnNetwork(PnNetwork *pnNetwork)
{
  pnNetwork_ = pnNetwork;

  // Enable antialiasing
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  setScene(pnNetwork_);
}

/*******************************************************************************
 * getPnNetwork.
 ******************************************************************************/
PnNetwork *PnView::getPnNetwork()
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
 * mousePressEvent.
 ******************************************************************************/
//void PnView::mousePressEvent(QMouseEvent *event)
//{
//  if(event->button() == Qt::MiddleButton) {
//    event->accept();
//    setDragMode(ScrollHandDrag);
//  } else {
//    event->ignore();
//  }
//}

/*******************************************************************************
 * mouseReleaseEvent.
 ******************************************************************************/
//void PnView::mouseReleaseEvent(QMouseEvent *event)
//{
//  if(event->button() == Qt::MiddleButton) {
//    event->accept();
//    setDragMode(RubberBandDrag);
//  } else {
//    event->ignore();
//  }
//}

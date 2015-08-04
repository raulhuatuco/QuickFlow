#include "pnview.h"

#include <QColor>
#include <QBrush>

#include "PnGraphics/pninfobox.h"

PnView::PnView(QWidget *parent) : QGraphicsView(parent), pnNetwork_(NULL) {
  setDragMode(ScrollHandDrag);
}

PnView::~PnView() {}

void PnView::zoomIn() { scale(1.0 + kZoomStep, 1.0 + kZoomStep); }

void PnView::zoomOut() { scale(1.0 - kZoomStep, 1.0 - kZoomStep); }

void PnView::zoomFit() { fitInView(sceneRect(), Qt::KeepAspectRatio); }

void PnView::setPnNetwork(PnNetwork *pnNetwork) {
  pnNetwork_ = pnNetwork;

  // Enable antialiasing
  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

  // Set scene
  setScene(pnNetwork_);
}

PnNetwork *PnView::getPnNetwork() { return pnNetwork_; }

void PnView::wheelEvent(QWheelEvent *event) {
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

  if (event->delta() > 0) {
    scale(1.0 + kZoomStep, 1.0 + kZoomStep);
  } else {
    scale(1.0 - kZoomStep, 1.0 - kZoomStep);
  }
}

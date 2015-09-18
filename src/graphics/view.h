#ifndef PNVIEW_H
#define PNVIEW_H

#include <QtCore>
#include <QtWidgets>
#include <QtGui>

#include "pn/network.h"

class PnView : public QGraphicsView
{
  Q_OBJECT

public:
  PnView(QWidget *parent);
  ~PnView();

  void zoomIn();
  void zoomOut();
  void zoomFit();

  void setPnNetwork(Network *pnNetwork);
  Network *getPnNetwork();

protected:
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:
  static const qreal kZoomStep = 0.05;
  QPoint oldPos;
  Network *pnNetwork_;
};

#endif  // PNVIEW_H

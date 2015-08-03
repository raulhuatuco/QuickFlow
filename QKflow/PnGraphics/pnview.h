#ifndef PNVIEW_H
#define PNVIEW_H

#include <QWidget>
#include <QWheelEvent>
#include <QGraphicsView>

#include "PnGraphics/pnnetwork.h"

class PnView : public QGraphicsView {
  Q_OBJECT

 public:
  PnView(QWidget *parent);
  ~PnView();

  void zoomIn();
  void zoomOut();
  void zoomFit();

  void setPnNetwork(PnNetwork *pnNetwork);
  PnNetwork *getPnNetwork();

 protected:
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

 private:
  static const qreal kZoomStep = 0.05;

  PnNetwork *pnNetwork_;


};

#endif // PNVIEW_H

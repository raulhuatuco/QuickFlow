#ifndef PNCABLE_H
#define PNCABLE_H

#include "PnGraphics/pnline.h"

class PnCable : public PnLine {
  Q_OBJECT

 public:
  static const int kCableWidth = 3;

  PnCable();
  PnCable(PnBar *noI, PnBar *noF);
  ~PnCable();

  void setNodes(PnBar *noI, PnBar *noF);

  QString lineType() Q_DECL_OVERRIDE;
  QRectF boundingRect() const Q_DECL_OVERRIDE;
  QPainterPath shape() const Q_DECL_OVERRIDE;

 protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);

 private:
  QPolygonF selectionArea;
};

#endif  // PNCABLE_H

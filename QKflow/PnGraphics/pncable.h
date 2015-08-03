#ifndef PNCABLE_H
#define PNCABLE_H

#include "PnGraphics/pnline.h"

class PnCable : public PnLine {
  Q_OBJECT

 public:
  static const int kCableWidth = 2;

  PnCable(PnBar *noI, PnBar *noF);
  ~PnCable();

  QString lineType() Q_DECL_OVERRIDE;
  QRectF boundingRect() const Q_DECL_OVERRIDE;
  QPainterPath shape() const Q_DECL_OVERRIDE;
 protected:
  virtual void paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget);

 private:
  QPolygonF selectionArea;
};

#endif // PNCABLE_H

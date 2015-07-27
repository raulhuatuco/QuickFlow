#ifndef PNCABLE_H
#define PNCABLE_H

#include "PnGraphics/pnline.h"

class PnCable : public PnLine
{
  Q_OBJECT

public:
  PnCable();
  ~PnCable();

  void setNodes(PnBar *noI, PnBar *noF);

  QByteArray generateKflowData() Q_DECL_OVERRIDE;
  QRectF boundingRect() const Q_DECL_OVERRIDE;

  QColor cableColor() const;
  void setCableColor(const QColor &cableColor);

  QPen cablePen() const;
  void setCablePen(const QPen &cablePen);

  QLineF cablePos() const;
  void setCablePos(const QLineF &cablePos);

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);

private:
  QColor cableColor_;
  QPen   cablePen_;
  QLineF cablePos_;
};

#endif // PNCABLE_H

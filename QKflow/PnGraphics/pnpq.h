#ifndef PNPQ_H
#define PNPQ_H

#include"pnbar.h"

class PnPq : public PnBar
{
  Q_OBJECT

public:
  static QPixmap *ico;
  static QPixmap *icoSelected;

  PnPq();
  ~PnPq();

  virtual QByteArray generateKflowData() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;
};

#endif // PNPQ_H

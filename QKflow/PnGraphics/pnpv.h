#ifndef PNPV_H
#define PNPV_H

#include "PnGraphics/pnbar.h"

class PnPv : public PnBar
{
  Q_OBJECT

public:
  static const int kIconSize = 10;

  PnPv(uint32_t id);
  ~PnPv();

  virtual QString barType() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;

};

#endif // PNPV_H

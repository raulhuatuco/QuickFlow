#ifndef PNPQ_H
#define PNPQ_H

#include <stdint.h>

#include"pnbar.h"

class PnPq : public PnBar
{
  Q_OBJECT

public:
  static const int kIconSize = 10;

  PnPq();
  PnPq(uint32_t id);
  ~PnPq();

  virtual QString barType() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;
};

#endif // PNPQ_H

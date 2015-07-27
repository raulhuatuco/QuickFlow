#ifndef PNSLACK_H
#define PNSLACK_H

#include <QGraphicsObject>
#include <QByteArray>
#include <complex>

#include "pnbar.h"

class PnSlack : public PnBar
{
  Q_OBJECT

public:
  static QPixmap *ico;
  static QPixmap *icoSelected;

  PnSlack();
  ~PnSlack();

  virtual QByteArray generateKflowData() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;
};

#endif // PNSLACK_H

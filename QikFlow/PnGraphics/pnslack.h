#ifndef PNSLACK_H
#define PNSLACK_H

#include <QGraphicsObject>
#include <QByteArray>
#include <complex>

#include "pnbar.h"

class PnSlack : public PnBar {
  Q_OBJECT

 public:
  static const int kIconSize = 15;

  PnSlack();
  ~PnSlack();

  void setMaxGeneration(double maxGeneration);
  double maxGeneration();

  virtual QString barType() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

 protected:
  double maxGeneration_;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;
};

#endif  // PNSLACK_H

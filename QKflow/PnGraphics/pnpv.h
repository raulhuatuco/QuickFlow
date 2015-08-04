#ifndef PNPV_H
#define PNPV_H

#include "PnGraphics/pnbar.h"

class PnPv : public PnBar {
  Q_OBJECT

 public:
  static const int kIconSize = 10;

  PnPv();
  PnPv(uint32_t id);
  ~PnPv();

  void setMaxQGenerated(double maxQGenerated);
  double getMaxQGenerated();

  void setMinQGenerated(double minQGenerated);
  double getMinQGenerated();

  virtual QString barType() Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

 protected:
  double maxQGenerated_;
  double minQGenerated_;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;
};

#endif  // PNPV_H

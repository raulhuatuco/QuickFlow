#ifndef PNLINE_H
#define PNLINE_H

#include <QGraphicsObject>
#include <QPen>
#include <complex>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pninfobox.h"

QT_BEGIN_NAMESPACE
class PnBar;
QT_END_NAMESPACE

class PnLine : public QGraphicsObject {
  Q_OBJECT

 public:
  PnLine(PnBar *noI, PnBar *noF);
  ~PnLine();

  // Input
  unsigned int Id();
  void setId(unsigned int id);

  PnBar *NoI();
  PnBar *NoF();
  void setNodes(PnBar *noI, PnBar *noF);

  std::complex<double> Impedance();
  void setImpedance(std::complex<double> z);

  std::complex<double> Admittance();
  void setAdmittance(std::complex<double> y);

  double MaxLoad();
  void setMaxLoad(double maxLoad);

  // Output
  std::complex<double> Current();
  void setCurrent(std::complex<double> i);

  std::complex<double> Load();

  virtual QString lineType() = 0 Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const = 0 Q_DECL_OVERRIDE;
  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

 protected:
  unsigned int id_;
  PnBar *noI_;
  PnBar *noF_;
  std::complex<double> z_;
  std::complex<double> i_;
  double maxLoad_;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) = 0 Q_DECL_OVERRIDE;

 private:
  PnInfoBox *infobox_;
};

#endif  // PNLINE_H

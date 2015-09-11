#ifndef PNLINE_H
#define PNLINE_H

#include <QGraphicsObject>
#include <QPen>
#include <complex>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pninfoline.h"

QT_BEGIN_NAMESPACE
class PnBar;
class PnInfoLine;
QT_END_NAMESPACE

class PnLine : public QGraphicsObject
{
  Q_OBJECT

  static const qreal kCableWidth = 3;

public:
  PnLine();
  ~PnLine();

// Parent bars
  PnBar *noI();
  PnBar *noF();

  virtual void setNodes(PnBar *noI, PnBar *noF);

  // Impedance
  std::complex<double> Zaa;
  std::complex<double> Zab;
  std::complex<double> Zac;
  std::complex<double> Zan;

  std::complex<double> Zbb;
  std::complex<double> Zbc;
  std::complex<double> Zbn;

  std::complex<double> Zcc;
  std::complex<double> Zcn;

  std::complex<double> Znn;

  // Admittance
  std::complex<double> Yaa();
  std::complex<double> Yab();
  std::complex<double> Yac();
  std::complex<double> Yan();

  std::complex<double> Ybb();
  std::complex<double> Ybc();
  std::complex<double> Ybn();

  std::complex<double> Ycc();
  std::complex<double> Ycn();

  std::complex<double> Ynn();

  // Current
  std::complex<double> Ia;
  std::complex<double> Ib;
  std::complex<double> Ic;
  std::complex<double> In;

  // Loss
  std::complex<double> La();
  std::complex<double> Lb();
  std::complex<double> Lc();
  std::complex<double> Ln();

  // Length
  double length;

  void updatePosition();

  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
  QPainterPath shape() const Q_DECL_OVERRIDE;
  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

signals:

  void eventDoubleClick();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  PnBar *noI_;
  PnBar *noF_;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;

private:
  QPolygonF selectionArea;
  QLineF coords;
  PnInfoLine *pnInfoLine_;
};

#endif  // PNLINE_H

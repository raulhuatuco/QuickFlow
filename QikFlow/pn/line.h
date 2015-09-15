#ifndef PNLINE_H
#define PNLINE_H

#include <QGraphicsObject>
#include <QPen>
#include <QJsonObject>

#include <complex>

#include "pn/bar.h"
#include "pn/infoline.h"
#include "pn/network.h"

QT_BEGIN_NAMESPACE
class PnBar;
class PnInfoLine;
class PnNetwork;
QT_END_NAMESPACE

class PnLine : public QGraphicsObject
{
  Q_OBJECT

public:
  static const qreal kCableWidth = 3;

  // Parents Id.
  uint32_t noI;
  uint32_t noF;

  // Network.
  PnNetwork *pnNetwork;

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

  // Current
  std::complex<double> Ia;
  std::complex<double> Ib;
  std::complex<double> Ic;
  std::complex<double> In;

  // Length
  double length;

  PnLine();
  ~PnLine();

  // Parent bars pointer.
  PnBar *pNoI();
  PnBar *pNoF();

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

  // Loss
  std::complex<double> La();
  std::complex<double> Lb();
  std::complex<double> Lc();
  std::complex<double> Ln();

  void setNodes(PnBar *pNoI, PnBar *pNoF);

  // Cast.
  QJsonObject toJson();
  void fromJson(QJsonObject &lineJson);

  void updatePosition();

  virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
  QPainterPath shape() const Q_DECL_OVERRIDE;
  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

signals:

  void eventDoubleClick();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  PnBar *pNoI_;
  PnBar *pNoF_;
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) Q_DECL_OVERRIDE;

private:
  QPolygonF selectionArea;
  QLineF coords;
  PnInfoLine *pnInfoLine_;
};

#endif  // PNLINE_H

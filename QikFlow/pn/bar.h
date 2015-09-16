#ifndef PNBAR_H
#define PNBAR_H

#include <QGraphicsObject>
#include <QByteArray>
#include <QList>
#include <QJsonObject>
#include <armadillo>

#include <complex>
#include <stdint.h>

#include "pn/line.h"
#include "pn/infobar.h"
#include "pn/network.h"

QT_BEGIN_NAMESPACE
class PnLine;
class PnInfoBar;
class PnNetwork;
QT_END_NAMESPACE

using std::complex;
using arma::Col;
using arma::cx_double;

class PnBar : public QGraphicsObject
{
  Q_OBJECT

public:
  static const uint32_t kInvalidId = 0xFFFFFFFFU;
  static const int32_t kIconSize = 15;

  uint32_t id;

  PnNetwork *pnNetwork;

// Input parameters.

  Col<cx_double> &V() const;

  complex<double> Va;
  complex<double> Vb;
  complex<double> Vc;

  complex<double> Sga;
  complex<double> Sgb;
  complex<double> Sgc;

  complex<double> Sla;
  complex<double> Slb;
  complex<double> Slc;

  complex<double> Ia();
  complex<double> Ib();
  complex<double> Ic();

  // Output parameters.
  complex<double> rVa;
  complex<double> rVb;
  complex<double> rVc;

  complex<double> rSga;
  complex<double> rSgb;
  complex<double> rSgc;

  complex<double> rSla;
  complex<double> rSlb;
  complex<double> rSlc;

  complex<double> rIa();
  complex<double> rIb();
  complex<double> rIc();

  QList<PnLine *> lines;

  PnBar();
  ~PnBar();

  // Lines.
  void addLine(PnLine *line);
  void removeLine(PnLine *line);
  void removeLines();

  // Conversion.
  // PerUnit.
  void toPerUnit();
  void toBaseUnit();

  //Json.
  QJsonObject toJson();
  void fromJson(QJsonObject &jsonBar);


  // Graphics.
  QRectF boundingRect() const Q_DECL_OVERRIDE;

signals:
  void eventDoubleClick();

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) Q_DECL_OVERRIDE;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  Col<cx_double> V_;
  Col<cx_double> Sg_;
  Col<cx_double> Sl_;

  Col<cx_double> rV_;
  Col<cx_double> rSg_;
  Col<cx_double> rSl_;

  PnInfoBar *pnInfoBar_;

  void drawSlack(QPainter *painter);
  void drawPq(QPainter *painter);
};

#endif  // PNBAR_H

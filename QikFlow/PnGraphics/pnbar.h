#ifndef PNBAR_H
#define PNBAR_H

#include <QGraphicsObject>
#include <QByteArray>
#include <QList>
#include <complex>
#include <stdint.h>

#include "PnGraphics/pnline.h"
#include "PnGraphics/pninfobar.h"

QT_BEGIN_NAMESPACE
class PnLine;
class PnInfoBar;
QT_END_NAMESPACE

using std::complex;

class PnBar : public QGraphicsObject
{
  Q_OBJECT

public:
  static const uint32_t kInvalidId = 0xFFFFFFFFU;
  static const int32_t kIconSize = 15;

  PnBar();
  ~PnBar();

  // Input parameters.
  uint32_t id;

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

  // Lines
  void addLine(PnLine *line);
  void removeLine(PnLine *line);
  void removeLines();

// Graphics
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
  QList<PnLine *> lines;
  PnInfoBar *pnInfoBar_;

  void drawSlack(QPainter *painter);
  void drawPq(QPainter *painter);
};

#endif  // PNBAR_H

#ifndef PNBAR_H
#define PNBAR_H


#include <QGraphicsObject>
#include <QByteArray>
#include <QList>
#include <complex>

#include "PnGraphics/pnline.h"

QT_BEGIN_NAMESPACE
class PnLine;
QT_END_NAMESPACE

using std::complex;

class PnBar : public QGraphicsObject
{
  Q_OBJECT

public:


  PnBar();
  ~PnBar();

  u_int32_t getId();
  complex<double> getV();
  complex<double> getSg();
  complex<double> getSl();
  complex<double> getI();


  void setId(unsigned int id);
  void setV(complex<double> v);
  void setSg(complex<double> sg);
  void setSl(complex<double>sl);

  void addLine(PnLine *line);
  void removeLine(PnLine *line);
  void removeLines();

  virtual QByteArray generateKflowData() = 0 Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const = 0 Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) = 0 Q_DECL_OVERRIDE;

  unsigned int id_;
  complex<double> v_;
  complex<double> sg_;
  complex<double> sl_;

private:
  QList<PnLine *> lines_;


};

#endif // PNBAR_H

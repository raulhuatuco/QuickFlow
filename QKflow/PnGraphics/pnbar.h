#ifndef PNBAR_H
#define PNBAR_H

#include <QGraphicsObject>
#include <QByteArray>
#include <QList>
#include <QToolTip>
#include <complex>
#include <stdint.h>

#include "PnGraphics/pnline.h"
#include "PnGraphics/pninfobox.h"

QT_BEGIN_NAMESPACE
class PnLine;
QT_END_NAMESPACE

using std::complex;

class PnBar : public QGraphicsObject {
  Q_OBJECT

 public:
  static const uint32_t kInvalidId = 0xFFFFFFFFU;

  PnBar(uint32_t id);
  ~PnBar();

  // Input parameters.
  uint32_t Id();
  void setId(uint32_t id);

  complex<double> inputV();
  void setInputV(complex<double> v);

  complex<double> inputSg();
  void setInputSg(complex<double> sg);

  complex<double> inputSl();
  void setInputSl(complex<double> sl);

  // Output parameters.
  complex<double> outputV();
  void setOutputV(complex<double> v);

  complex<double> outputSg();
  void setOutputSg(complex<double> sg);

  complex<double> outputI();
  void setOutputI(complex<double> i);

  // Lines
  void addLine(PnLine *line);
  void removeLine(PnLine *line);
  void removeLines();

  virtual QString barType() = 0 Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const = 0 Q_DECL_OVERRIDE;

 protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) = 0 Q_DECL_OVERRIDE;

  virtual QVariant itemChange(GraphicsItemChange change,
                              const QVariant &value) Q_DECL_OVERRIDE;

  uint32_t id_;
  complex<double> inputV_;
  complex<double> inputSg_;
  complex<double> inputSl_;

  complex<double> outputV_;
  complex<double> outputSg_;

 private:
  QList<PnLine *> lines_;
  PnInfoBox *infobox_;
};

#endif  // PNBAR_H

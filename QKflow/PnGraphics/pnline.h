#ifndef PNLINE_H
#define PNLINE_H

#include <QGraphicsObject>
#include <QPen>

#include "PnGraphics/pnbar.h"

QT_BEGIN_NAMESPACE
class PnBar;
QT_END_NAMESPACE

class PnLine : public QGraphicsObject
{
  Q_OBJECT

public:
  PnLine();
  ~PnLine();

  PnBar *getNoI() {return noI_;}
  PnBar *getNoF() {return noF_;}

  virtual void setNodes(PnBar *noI, PnBar *noF);
  virtual QByteArray generateKflowData() = 0 Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const = 0 Q_DECL_OVERRIDE;

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) = 0 Q_DECL_OVERRIDE;

private:
  PnBar *noI_;
  PnBar *noF_;
};

#endif // PNLINE_H

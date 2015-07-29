#ifndef PNLINE_H
#define PNLINE_H

#include <QGraphicsObject>
#include <QPen>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pninfobox.h"

QT_BEGIN_NAMESPACE
class PnBar;
QT_END_NAMESPACE

class PnLine : public QGraphicsObject
{
  Q_OBJECT

public:
  PnLine(PnBar *noI, PnBar *noF);
  ~PnLine();

  PnBar *getNoI() {return noI_;}
  PnBar *getNoF() {return noF_;}


  virtual QString lineType() = 0 Q_DECL_OVERRIDE;
  virtual QRectF boundingRect() const = 0 Q_DECL_OVERRIDE;
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

protected:
  PnBar *noI_;
  PnBar *noF_;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) = 0 Q_DECL_OVERRIDE;

private:
  PnInfoBox *infobox_;
};

#endif // PNLINE_H

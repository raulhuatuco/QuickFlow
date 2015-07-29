#ifndef PNINFOBOX_H
#define PNINFOBOX_H

#include <QGraphicsItem>
#include <QPoint>

class PnInfoBox : public QGraphicsItem
{
public:

  static const int kBoxWidth = 180;
  static const int kBoxHeight = 140;
  static const int kMargin = 10;

  PnInfoBox(qreal px, qreal py, QString txt);
  ~PnInfoBox();

  QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)Q_DECL_OVERRIDE;

private:
  QString txt_;
};

#endif // PNINFOBOX_H

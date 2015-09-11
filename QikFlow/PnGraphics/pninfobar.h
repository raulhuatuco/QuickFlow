#ifndef PNINFOBAR_H
#define PNINFOBAR_H

#include <QGraphicsItem>
#include <QPoint>

#include <PnGraphics/pnbar.h>
QT_BEGIN_NAMESPACE
class PnBar;
QT_END_NAMESPACE

class PnInfoBar : public QGraphicsItem
{
public:
  // Box options.
  static const qreal kBoxWidth = 660.0;
  static const qreal kBoxHeight = 150.0;
  static const qreal kLineWidth = 2.0;

  // Head options.
  static const qreal kMarginHeadTop = 5.0;
  static const qreal kMarginHeadBot = 5.0;

  // Inner box options.
  static const qreal kMarginTop = 5.0;
  static const qreal kMarginBot = 5.0;
  static const qreal kMarginLeft = 5.0;
  static const qreal kMarginRight = 5.0;

  // Table options
  static const qreal kTableLineWidth = 1.0;
  static const qreal kTextToLinePad = 1.0;
  static const int kTableRows = 4.0;
  static const int kTableColums = 5.0;

  // Base options.
  static const qreal kBoxBaseHeight = 30.0;
  static const qreal kBoxBaseWidth = 60.0;

  PnInfoBar(PnBar *bar);
  ~PnInfoBar();

  QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  PnBar *bar_;
};

#endif  // PNINFOBOX_H

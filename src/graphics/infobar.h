#ifndef PNINFOBAR_H
#define PNINFOBAR_H

#include <QGraphicsItem>
#include <QPoint>

#include "models/bar.h"
QT_BEGIN_NAMESPACE
class Bar;
QT_END_NAMESPACE

class InfoBar : public QGraphicsItem
{
public:
  static const int kTableRows;
  static const int kTableColums;

  // Box options.
  static const qreal kBoxWidth;
  static const qreal kBoxHeight;
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


  // Base options.
  static const qreal kBoxBaseHeight = 30.0;
  static const qreal kBoxBaseWidth = 60.0;

  InfoBar(Bar *bar);
  ~InfoBar();

  QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  Bar *bar_;
};

#endif  // PNINFOBOX_H

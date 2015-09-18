#ifndef PNINFOLINE_H
#define PNINFOLINE_H

#include <QGraphicsItem>
#include <QPoint>

#include "pn/line.h"

QT_BEGIN_NAMESPACE
class Line;
QT_END_NAMESPACE

class InfoLine : public QGraphicsItem
{
public:
  // Box options.
  static const qreal kBoxWidth = 400.0;
  static const qreal kBoxHeight = 200.0;
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
  static const int kTableRows = 5.0;
  static const int kTableColums = 3.0;

  // Base options.
  static const qreal kBoxBaseHeight = 30.0;
  static const qreal kBoxBaseWidth = 60.0;

  InfoLine(Line *line);
  ~InfoLine();

  QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;

private:
  Line *line_;
};

#endif  // PNINFOBOX_H

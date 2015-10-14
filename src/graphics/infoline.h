#ifndef PNINFOLINE_H
#define PNINFOLINE_H

#include <QGraphicsItem>
#include <QPoint>

#include "models/line.h"

QT_BEGIN_NAMESPACE
class Line;
QT_END_NAMESPACE

class InfoLine : public QGraphicsItem
{
public:
  static const int kTableRows = 4;
  static const int kTableColums = 3;

  // Box options.
  static const qreal kBoxWidth = 135.0*kTableColums;
  static const qreal kBoxHeight = 37.5*kTableRows;
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

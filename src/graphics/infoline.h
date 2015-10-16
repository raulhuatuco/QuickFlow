#ifndef GRAPHICS_INFOLINE_H
#define GRAPHICS_INFOLINE_H

#include <QGraphicsItem>
#include <QPoint>
#include "models/line.h"

QT_BEGIN_NAMESPACE
class Line;
QT_END_NAMESPACE

class InfoLine : public QGraphicsItem
{
public:
  static const int kTableRows;
  static const int kTableColums;

  // Box options.
  static const double kBoxWidth;
  static const double kBoxHeight;
  static const double kLineWidth;

  // Head options.
  static const double kMarginHeadTop;
  static const double kMarginHeadBot;

  // Inner box options.
  static const double kMarginTop;
  static const double kMarginBot;
  static const double kMarginLeft;
  static const double kMarginRight;

  // Base options.
  static const double kBoxBaseHeight;
  static const double kBoxBaseWidth;

  InfoLine(Line *line);
  ~InfoLine();

  QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) Q_DECL_OVERRIDE;
  void setHeadFont(QPainter *painter);
  void setNormalFont(QPainter *painter);
  void drawMainRectangle(QRectF mainRect, double lineWidth, QPainter *painter);
  void drawHeadSeparator(QRectF headRect, QPainter *painter);
  void drawHeadText(QRectF headRect, double marginTop, QString &text,
                    QPainter *painter);
                    
private:
  Line *line_;
};

#endif  // GRAPHICS_INFOLINE_H

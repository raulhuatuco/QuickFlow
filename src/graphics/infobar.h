#ifndef GRAPHICS_INFOBAR_H
#define GRAPHICS_INFOBAR_H

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPoint>
#include "models/bar.h"

QT_BEGIN_NAMESPACE
class Bar;
QT_END_NAMESPACE

class InfoBar : public QGraphicsObject
{
Q_OBJECT

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

  InfoBar(Bar *bar);
  ~InfoBar();

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
  Bar *bar_;
};

#endif  // GRAPHICS_INFOBAR_H

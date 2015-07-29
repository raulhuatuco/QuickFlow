#include "pnview.h"

#include <QColor>
#include <QBrush>

#include "PnGraphics/pninfobox.h"

PnView::PnView(QWidget *parent)
  : QGraphicsView(parent) {

    /* Adjust Scene options */
      // Create Scene
      QGraphicsScene *scene = new QGraphicsScene(this);

      // Set background to black

      QBrush bkbrush;
      bkbrush.setColor(QColor::fromRgbF(0.0,1.0,0.0, 0.2));
      bkbrush.setStyle(Qt::SolidPattern);

      scene->setBackgroundBrush (bkbrush);

      // Enable antialiasing
      setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

      // Set scene
      setScene(scene);

}

PnView::~PnView()
{

}


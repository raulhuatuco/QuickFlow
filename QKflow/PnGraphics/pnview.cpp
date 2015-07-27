#include "pnview.h"

PnView::PnView(QWidget *parent)
  : QGraphicsView(parent) {
  /* Adjust Scene options */
      // Create Scene
      QGraphicsScene *scene = new QGraphicsScene(this);

      // Set background to black

      scene->setBackgroundBrush (Qt::black);

      // Enable antialiasing
      setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

      scale (0.5, 0.5);
      // Set scene
      setScene(scene);
}

PnView::~PnView()
{

}


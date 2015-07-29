#include "pnline.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>

PnLine::PnLine(PnBar *noI, PnBar *noF)
    : noI_(noI), noF_(noF), infobox_(NULL)
{
  setFlag(ItemIsSelectable);
  setZValue(0.0);
  noI->addLine(this);
  noF->addLine(this);
}

PnLine::~PnLine()
{
    if (infobox_ != NULL)
        scene()->removeItem(infobox_);
}

QVariant PnLine::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedChange) {
    if(value == true) {
            qreal px = (noI_->pos().x() + noF_->pos().x())/2;
            qreal py = (noI_->pos().y() + noF_->pos().y())/2;

            QString txt = "Line : ";

            infobox_ = new PnInfoBox(px, py,txt);
            scene()->addItem(infobox_);

        } else{
            scene()->removeItem(infobox_);
            infobox_ = NULL;
        }
    }

    return QGraphicsObject::itemChange(change, value);
}

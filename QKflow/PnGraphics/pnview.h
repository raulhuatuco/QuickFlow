#ifndef PNVIEW_H
#define PNVIEW_H

#include <QWidget>
#include <QWheelEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHash>
#include <QSignalMapper>
#include <QObject>
#include <QStack>
#include <QInputDialog>

class PnView : public QGraphicsView
{
  Q_OBJECT

public:
  PnView(QWidget *parent);
  ~PnView();
};

#endif // PNVIEW_H

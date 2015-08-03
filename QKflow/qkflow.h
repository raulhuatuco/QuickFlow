#ifndef QKFLOW_H
#define QKFLOW_H

#include <QMainWindow>

#include "PnGraphics/pnnetwork.h"

#include "windowaddslack.h"
#include "windowaddpq.h"

namespace Ui {
class QKflow;
}

class QKflow : public QMainWindow {
  Q_OBJECT

 public:
  explicit QKflow(QWidget *parent = 0);
  ~QKflow();

 private slots:
  void on_actionZoomIn_triggered();

  void on_actionZoomOut_triggered();

  void on_actionZoomFit_triggered();

  void on_actionSlack_triggered();

  void on_actionPQBar_triggered();

  void on_actionPVBar_triggered();

 private:
  Ui::QKflow *ui;
  WindowAddSlack *windowAddSlack;
  WindowAddPq *windowAddPq;

  PnNetwork *pnNetwork_;
};

#endif // QKFLOW_H

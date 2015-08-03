#ifndef WINDOWADDPQ_H
#define WINDOWADDPQ_H

#include <QDialog>

#include "PnGraphics/pnpq.h"

namespace Ui {
class WindowAddPq;
}

class WindowAddPq : public QDialog {
  Q_OBJECT

 public:
  explicit WindowAddPq(QWidget *parent = 0, PnPq *pq = NULL);
  ~WindowAddPq();

 private slots:
  void on_btnOk_clicked();

  void on_btnCancel_clicked();

 private:
  Ui::WindowAddPq *ui;
  PnPq *pq_;
};

#endif // WINDOWSADDPQ_H

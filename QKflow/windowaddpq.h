#ifndef WINDOWADDPQ_H
#define WINDOWADDPQ_H

#include <QDialog>

#include "PnGraphics/pnnetwork.h"

namespace Ui {
class WindowAddPq;
}

class WindowAddPq : public QDialog {
  Q_OBJECT

 public:
  explicit WindowAddPq(QWidget *parent = 0);
  ~WindowAddPq();

  void setNetwork(PnNetwork *pnNetwork);

 private slots:
  void on_btnOk_clicked();

  void on_btnCancel_clicked();

 private:
  Ui::WindowAddPq *ui;
  PnNetwork *pnNetwork_;
};

#endif  // WINDOWSADDPQ_H

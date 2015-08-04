#ifndef WINDOWADDPV_H
#define WINDOWADDPV_H

#include <QDialog>

#include "PnGraphics/pnnetwork.h"

namespace Ui {
class WindowAddPv;
}

class WindowAddPv : public QDialog {
  Q_OBJECT

 public:
  explicit WindowAddPv(QWidget *parent = 0);
  ~WindowAddPv();
  void setNetwork(PnNetwork *pnNetwork);

private slots:
  void on_btnCancel_clicked();

  void on_btnOk_clicked();

private:
  Ui::WindowAddPv *ui;
  PnNetwork *pnNetwork_;
};

#endif  // WINDOWADDPV_H

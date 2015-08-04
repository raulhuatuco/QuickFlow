#ifndef WINDOWADDSLACK_H
#define WINDOWADDSLACK_H

#include <QDialog>

#include "PnGraphics/pnnetwork.h"

namespace Ui {
class WindowAddSlack;
}

class WindowAddSlack : public QDialog {
  Q_OBJECT

 public:
  explicit WindowAddSlack(QWidget *parent = 0);
  ~WindowAddSlack();

  void setNetwork(PnNetwork *pnNetwork);

 private slots:
  void on_btnCancel_clicked();

  void on_btnOk_clicked();

 private:
  Ui::WindowAddSlack *ui;
  PnNetwork *pnNetwork_;
};

#endif  // WINDOWADDSLACK_H

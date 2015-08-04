#ifndef WINDOWADDCABLE_H
#define WINDOWADDCABLE_H

#include <QDialog>

#include "PnGraphics/pnnetwork.h"

namespace Ui {
class WindowAddCable;
}

class WindowAddCable : public QDialog {
  Q_OBJECT

 public:
  explicit WindowAddCable(QWidget *parent = 0);
  ~WindowAddCable();

  void setNetwork(PnNetwork *pnNetwork);

private slots:
  void on_btnCancel_clicked();

  void on_btnOk_clicked();

private:
  Ui::WindowAddCable *ui;
  PnNetwork *pnNetwork_;
};

#endif  // WINDOWADDCABLE_H

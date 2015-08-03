#ifndef WINDOWADDPV_H
#define WINDOWADDPV_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

#include "PnGraphics/pnnetwork.h"

class WindowAddPv : public QDialog {
  Q_OBJECT

 public:

  WindowAddPv(QWidget *parent);
  ~WindowAddPv();

  void setNetwork(PnNetwork *network);

 private slots:
  void on_btnOk_clicked();

  void on_btnCancel_clicked();

 private:
  PnNetwork *network_;

  QLineEdit *Pgen;
  QLineEdit *Pload;
  QLineEdit *Qload;
  QLineEdit *MaxQGeneration;
  QLineEdit *Px;
  QLineEdit *Py;
  QPushButton *btnOk;
  QPushButton *btnCancel;

};

#endif // WINDOWADDPV_H

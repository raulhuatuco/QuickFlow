#ifndef WINDOWKFLOWSETTINGS_H
#define WINDOWKFLOWSETTINGS_H

#include <QDialog>

#include "kflow.h"

namespace Ui {
class windowKflowSettings;
}

class WindowKflowSettings : public QDialog {
  Q_OBJECT

 public:
  explicit WindowKflowSettings(QWidget *parent = 0);
  ~WindowKflowSettings();

  void setKflow(Kflow *kflow);

 private slots:
  void on_btnCancel_clicked();

  void on_btnPath_clicked();

  void on_btnOk_clicked();

 private:
  Ui::windowKflowSettings *ui;
  Kflow *kflow_;
};

#endif  // WINDOWKFLOWSETTINGS_H

#ifndef QKFLOW_H
#define QKFLOW_H

#include <QMainWindow>
#include <QSettings>
#include <QString>

#include "PnGraphics/pnnetwork.h"
#include "kflow.h"

namespace Ui {
class QKflow;
}

class QKflow : public QMainWindow {
  Q_OBJECT

 public:
  static const QString kVersion;

  explicit QKflow(QWidget *parent = 0);
  ~QKflow();

 private slots:
  void on_actionZoomIn_triggered();

  void on_actionZoomOut_triggered();

  void on_actionZoomFit_triggered();

  void on_actionSlack_triggered();

  void on_actionPQBar_triggered();

  void on_actionPVBar_triggered();

  void on_actionCable_triggered();

  void on_actionRun_triggered();

  void on_actionSettings_triggered();

  void on_actionNew_triggered();

  void on_actionOpen_triggered();

 private:
  Ui::QKflow *ui;

  QSettings *settings;
  PnNetwork *pnNetwork_;
  Kflow *kflow;

  void loadSettings();
  void saveSettings();
  void createSettings();
  void upgradeSettings();
};

#endif  // QKFLOW_H

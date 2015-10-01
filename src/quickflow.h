#ifndef QUICKFLOW_H
#define QUICKFLOW_H

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "project.h"

namespace Ui
{
class QuickFlow;
}

class QuickFlow : public QMainWindow
{
  Q_OBJECT

public:

  explicit QuickFlow(QWidget *parent = 0);
  ~QuickFlow();

  bool isAltered();
  void setAltered(bool altered);

  void noProjectInterface();
  void workInterface();

private slots:
  void on_actionZoomIn_triggered();

  void on_actionZoomOut_triggered();

  void on_actionZoomFit_triggered();

  void on_actionExit_triggered();

  void on_actionNew_triggered();

  void on_actionOpen_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_actionClose_triggered();

  void on_actionAddBar_triggered();

  void on_actionAddLine_triggered();

  void on_editBar(QObject *bar);

  void on_editLine(QObject *line);

  void on_action_txt_type_1_triggered();

  void on_action_txt_type_2_triggered();

  void on_actionCespedes_triggered();

  void on_actionShirmoharmnadi_triggered();

  void on_actionPause_triggered();

  void on_actionSugiyama_triggered();

  void on_actionSugiyama_Fast_triggered();

  void on_actionMulti_level_triggered();

  void on_actionSearch_Bar_triggered();

  void on_action_txt_type_3_triggered();
  
  void on_actionRun_triggered();
  
protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::QuickFlow *ui;

  QSettings *settings;
  Project *project;

  bool altered_;

  void loadSettings();
  void saveSettings();
  void createSettings();
  void upgradeSettings();
  void connectSignals();
  void disconnectSignals();
};

#endif  // QKFLOW_H

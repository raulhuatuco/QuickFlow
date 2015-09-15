#ifndef QKFLOW_H
#define QKFLOW_H

#include <QMainWindow>
#include <QSettings>
#include <QString>

#include "project.h"

namespace Ui
{
class QKflow;
}

class QKflow : public QMainWindow
{
  Q_OBJECT

public:

  explicit QKflow(QWidget *parent = 0);
  ~QKflow();

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
  
protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
  Ui::QKflow *ui;

  QSettings *settings;
  Project *project;

  bool altered_;

  void loadSettings();
  void saveSettings();
  void createSettings();
  void upgradeSettings();
  void connectSignals();
  void disconnectSignals();
  bool import1(QString &fileName);
  bool import2(QString &fileName);
};

#endif  // QKFLOW_H

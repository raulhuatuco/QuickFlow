#ifndef WINDOWNEWPROJECT_H
#define WINDOWNEWPROJECT_H

#include <QDialog>
#include <QString>
#include <QDir>

#include "customtypes.h"

namespace Ui
{
class NewProject;
}

class NewProject : public QDialog
{
  Q_OBJECT

public:
  explicit NewProject(QWidget *parent = 0);
  ~NewProject();

  QString dataName;
  QString dataPath;

  u_int32_t dataMaxIterations;
  double dataMinError;
  double dataVoltageBase;
  double dataPowerBase;

  Unit::LengthUnit dataLengthUnit;
  Unit::ImpedanceUnit dataImpedanceUnit;
  Unit::VoltageUnit dataVoltageUnit;
  Unit::PowerUnit dataPowerUnit;

private slots:
  void on_name_textChanged(const QString &arg1);

  void on_path_textChanged(const QString &arg1);

  void on_buttonBox_accepted();

  void on_toolButton_clicked();

  void on_buttonBox_rejected();

private:
  Ui::NewProject *ui;
};

#endif  // WINDOWNEWPROJECT_H

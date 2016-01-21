#ifndef WINDOWBARPROPERTIES_H
#define WINDOWBARPROPERTIES_H

#include <QDialog>
#include "unit.h"
#include "project.h"

namespace Ui
{
class BarProperties;
}

class BarProperties : public QDialog
{
  Q_OBJECT

public:
  explicit BarProperties(QWidget *parent = 0);
  ~BarProperties();
  bool setOptions(Project *project, Bar *bar);
  Bar *bar();

private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();

  void on_network_currentIndexChanged(const QString &arg1);
  
private:
  Ui::BarProperties *ui;
  Project *project_;
  Bar *bar_;
  bool isNew;
};

#endif // WINDOWBARPROPERTIES_H

#ifndef WINDOWBARPROPERTIES_H
#define WINDOWBARPROPERTIES_H

#include <QDialog>

#include "pn/types.h"
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

  void setOptions(Project *project, PnBar *bar);

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::BarProperties *ui;
  Project *project_;
  PnBar *bar_;
  bool isNew;
};

#endif // WINDOWBARPROPERTIES_H

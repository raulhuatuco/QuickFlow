#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>

#include "pn/types.h"

#include "project.h"
#include "pn/line.h"
#include "pn/bar.h"

namespace Ui
{
class LineProperties;
}

class LineProperties : public QDialog
{
  Q_OBJECT

public:
  explicit LineProperties(QWidget *parent = 0);
  ~LineProperties();

  void setOptions(Project *project, PnLine *line);

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::LineProperties *ui;
  Project *project_;
  PnLine *line_;
  bool isNew;

  bool validImpedance(QLineEdit *input);
};

#endif // LINEPROPERTIES_H

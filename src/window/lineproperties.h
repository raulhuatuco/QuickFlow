#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>
#include "customtypes.h"
#include "project.h"
#include "models/line.h"
#include "models/bar.h"

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

  void setOptions(Project *project, Line *line);

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::LineProperties *ui;
  Project *project_;
  Line *line_;
  bool isNew;

  bool validImpedance(QLineEdit *input);
};

#endif // LINEPROPERTIES_H

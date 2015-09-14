#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QDialog>
#include <QMap>
#include <QLineEdit>

#include "unit.h"

#include "graphics/pnline.h"
#include "graphics/pnbar.h"

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

  Ui::LineProperties *ui;

  void setLine(PnLine *line, bool newLine);
  void setBarMap(QMap<uint32_t, PnBar *> &barMap);
  void setUnit(Unit::LengthUnit lengthUnit, Unit::ImpedanceUnit impedanceUnit);

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  PnLine *line_;
  bool newLine_;
  QMap<uint32_t, PnBar *> *barMap_;

  bool validImpedance(QLineEdit *input);
};

#endif // LINEPROPERTIES_H

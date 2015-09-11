#ifndef WINDOWBARPROPERTIES_H
#define WINDOWBARPROPERTIES_H

#include <QDialog>

#include "unit.h"
#include "PnGraphics/pnbar.h"

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

  void setBar(PnBar *bar, bool newBar);
  void setUnit(Unit::VoltageUnit voltageUnit, Unit::PowerUnit powerUnit);

private slots:
  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

private:
  Ui::BarProperties *ui;
  PnBar *bar_;
};

#endif // WINDOWBARPROPERTIES_H

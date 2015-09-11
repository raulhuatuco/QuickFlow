#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QJsonObject>

#include "unit.h"

#include "PnGraphics/pnnetwork.h"
#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

class Project
{
public:
  Project();
  ~Project();

  PnNetwork *pnNetwork;

  // Project options.
  QString name;
  QString filepath;
  uint32_t maxIterations();
  void setMaxIterations(uint32_t maxIterations);
  double minError();
  void setMinError(double minError);

  // Network options.
  double voltageBase();
  void setVoltageBase(double voltageBase);
  double powerBase();
  void setPowerBase(double powerBase);
  Unit::LengthUnit lengthUn();
  void setLengthUn(Unit::LengthUnit lengthUn);
  Unit::ImpedanceUnit impedanceUn();
  void setImpedanceUn(Unit::ImpedanceUnit impedanceUn);
  Unit::VoltageUnit voltageUn();
  void setVoltageUn(Unit::VoltageUnit voltageUn);
  Unit::PowerUnit powerUn();
  void setPowerUn(Unit::PowerUnit powerUn);

  bool save();
  bool saveAs(QString fileName);
  bool load();

private:
  QJsonObject barToJson(PnBar *bar);
  PnBar *barFromJson(QJsonObject &jsonBar);
  QJsonObject lineToJson(PnLine *line);
  PnLine *lineFromJson(QJsonObject &jsonLine);
};

#endif  // PROJECTSETTINGS_H

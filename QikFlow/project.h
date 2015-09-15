#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>

#include "pn/types.h"

#include "pn/network.h"
#include "pn/bar.h"
#include "pn/line.h"

class Project
{
public:
  Project();
  ~Project();

  PnNetwork *pnNetwork;

  // Project options.
  QString name;
  QString filePath;
  uint32_t maxIterations();
  void setMaxIterations(uint32_t maxIterations);
  double minError();
  void setMinError(double minError);

  // Network options.
  double voltageBase();
  void setVoltageBase(double voltageBase);
  double powerBase();
  void setPowerBase(double powerBase);
  Unit::LengthUnit lengthUnit();
  void setLengthUn(Unit::LengthUnit lengthUnit);
  Unit::ImpedanceUnit impedanceUnit();
  void setImpedanceUn(Unit::ImpedanceUnit impedanceUnit);
  Unit::VoltageUnit voltageUnit();
  void setVoltageUn(Unit::VoltageUnit voltageUnit);
  Unit::PowerUnit powerUnit();
  void setPowerUn(Unit::PowerUnit powerUnit);

  bool save();
  bool saveAs(QString fileName);
  bool load();

private:
  QJsonObject lineToJson(PnLine *line);
  PnLine *lineFromJson(QJsonObject &jsonLine);
};

#endif  // PROJECTSETTINGS_H

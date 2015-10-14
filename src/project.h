#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QHash>
#include "customtypes.h"
#include "models/network.h"

class Project
{
public:
  QHash<QString, Network *> networks;

  QString name;

  QString filePath;

  Project();

  ~Project();

  u_int32_t maxIterations();

  void setMaxIterations(u_int32_t maxIterations);

  double minError();

  void setMinError(double minError);

  double voltageBase();

  void setVoltageBase(double voltageBase);

  double powerBase();

  void setPowerBase(double powerBase);

  Unit::LengthUnit lengthUnit();

  void setLengthUn(Unit::LengthUnit unit);

  Unit::ImpedanceUnit impedanceUnit();

  void setImpedanceUn(Unit::ImpedanceUnit unit);

  Unit::VoltageUnit voltageUnit();

  void setVoltageUn(Unit::VoltageUnit unit);

  Unit::PowerUnit powerUnit();

  void setPowerUn(Unit::PowerUnit unit);

  Unit::CurrentUnit currentUnit();

  void setCurrentUnit(Unit::CurrentUnit unit);

  bool save();

  bool saveAs(QString fileName);

  bool load();

private:
};

#endif  // PROJECTSETTINGS_H

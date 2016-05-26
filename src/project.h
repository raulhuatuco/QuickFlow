#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <cstdint>

#include <QString>
#include <QHash>
#include <QPointer>
#include "unit.h"
#include "models/network.h"

class Project
{
public:
  QHash<QString, QPointer<Network> > networks;

  QString name;

  QString filePath;

  Project();

  ~Project();

  uint32_t maxIterations();

  void setMaxIterations(uint32_t maxIterations);

  double minError();

  void setMinError(double minError);

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

  bool exportData(QString &fileName);
  
private:
};

#endif  // PROJECTSETTINGS_H

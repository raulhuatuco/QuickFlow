#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QJsonObject>

#include "PnGraphics/pnnetwork.h"
#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

class Project {
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
  double lengthUn();
  void setLengthUn(double lengthUn);
  double impedanceUn();
  void setImpedanceUn(double impedanceUn);
  double voltageUn();
  void setVoltageUn(double voltageUn);
  double powerUn();
  void setPowerUn(double powerUn);

  bool save();
  bool saveAs(QString fileName);
  bool load();

 private:
  uint32_t maxIterations_;
  double minError_;

  QJsonObject barToJson(PnBar *bar);
  PnBar *barFromJson(QJsonObject &jsonBar);
  QJsonObject lineToJson(PnLine *line);
  PnLine *lineFromJson(QJsonObject &jsonLine);
};

#endif  // PROJECTSETTINGS_H

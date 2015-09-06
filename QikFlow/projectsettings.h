#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QJsonObject>

#include "PnGraphics/pnnetwork.h"
#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

class ProjectSettings {
 public:
  ProjectSettings();
  ~ProjectSettings();

  PnNetwork *pnNetwork();

  QString name();
  void setName(QString name);

  bool save(QString fileName);
  bool load(QString fileName);

 private:
  PnNetwork *pnNetwork_;
  QString name_;

  QJsonObject barToJson(PnBar *bar);
  PnBar *barFromJson(QJsonObject &jsonBar);
  QJsonObject lineToJson(PnLine *line);
  PnLine *lineFromJson(QJsonObject &jsonline);
};

#endif  // PROJECTSETTINGS_H

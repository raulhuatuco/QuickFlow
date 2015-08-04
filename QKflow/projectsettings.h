#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "PnGraphics/pnnetwork.h"

class ProjectSettings {
 public:
  ProjectSettings();
  ~ProjectSettings();

  void setNetwork(PnNetwork *_pnNetwork);
  PnNetwork *getNetwork();

 private:
  PnNetwork *pnNetwork_;
};

#endif  // PROJECTSETTINGS_H

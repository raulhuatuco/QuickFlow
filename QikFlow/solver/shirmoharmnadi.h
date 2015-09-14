#ifndef SHIRMOHARMNADI_H
#define SHIRMOHARMNADI_H

#include "graphics/pnbar.h"
#include "graphics/pnlayer.h"
#include "graphics/pnline.h"
#include "graphics/pnnetwork.h"

class Shirmoharmnadi
{
public:
  Shirmoharmnadi(PnNetwork *pnNetwork);
  ~Shirmoharmnadi();

  double duration;

  bool solve();

private:
  PnNetwork *pnNetwork_;
};

#endif // SHIRMOHARMNADI_H

#ifndef SHIRMOHARMNADI_H
#define SHIRMOHARMNADI_H

#include "pn/bar.h"
#include "pn/layer.h"
#include "pn/line.h"
#include "pn/network.h"

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

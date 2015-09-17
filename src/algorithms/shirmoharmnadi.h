#ifndef SHIRMOHARMNADI_H
#define SHIRMOHARMNADI_H

#include "pn/bar.h"
#include "pn/line.h"
#include "pn/network.h"

class Shirmoharmnadi
{
public:
  Shirmoharmnadi(Network *pnNetwork);
  ~Shirmoharmnadi();

  double duration;

  bool solve();

private:
  Network *pnNetwork_;
};

#endif // SHIRMOHARMNADI_H

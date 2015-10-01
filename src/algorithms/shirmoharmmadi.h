#ifndef SHIRMOHARMNADI_H
#define SHIRMOHARMNADI_H

#include "models/bar.h"
#include "models/line.h"
#include "graphics/network.h"

class Shirmoharmnadi
{
public:
  Shirmoharmnadi(Network *network);
  ~Shirmoharmnadi();

  double duration;

  bool solve();

private:
  Network *network_;
};

#endif // SHIRMOHARMNADI_H

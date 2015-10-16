#ifndef ALGORITHMS_SHIRMOHARMMADI_H
#define ALGORITHMS_SHIRMOHARMMADI_H

#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

class Shirmoharmmadi
{
public:
  Shirmoharmmadi(Network *network);
  ~Shirmoharmmadi();

  double duration;
  double slackGeneration;

  bool solve();

private:
  Network *network_;
};

#endif // ALGORITHMS_SHIRMOHARMMADI_H

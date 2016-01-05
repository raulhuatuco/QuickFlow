#ifndef BARREDUCTION_H
#define BARREDUCTION_H

#include <QPointer>
#include <QStack>
#include "models/network.h"
#include "models/bar.h"
#include "models/line.h"

class BarReduction
{
public:
  BarReduction();
  ~BarReduction();

  Network *getNetwork();
  void setNetwork(Network *network);

  int32_t reduce();
  void expand();

private:
  typedef struct {
    Line *equivalentLine;
    Bar *bar;
    Line *l1;
    Line *l2;
  } Droped;

  QPointer<Network> network_;
  QStack<Droped *> droped;

  double barAbsPower(Bar *bar);

};

#endif // BARREDUCTION_H

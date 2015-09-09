#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QMap>

#include "unit.h"

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

class PnNetwork : public QGraphicsScene {
 public:
  PnNetwork();
  ~PnNetwork();

  uint32_t maxIterations;
  double minError;

  double voltageBase;
  double powerBase;
  double currentBase;

  Unit::LengthUnit lengthUnit;
  Unit::ImpedanceUnit impedanceUnit;
  Unit::VoltageUnit voltageUnit;
  Unit::PowerUnit powerUnit;

  QMap<uint32_t, PnBar *> barMap;
  QMap<uint32_t, PnLine *> lineIMap;
  QMap<uint32_t, PnLine *> lineFMap;

  bool addBar(PnBar *bar);
  bool addLine(PnLine *line);

  PnBar *getBarById(uint32_t id);
  PnLine *getLineByNodes(uint32_t noI, uint32_t noF);

 private:
};

#endif  // PNNETWORK_H

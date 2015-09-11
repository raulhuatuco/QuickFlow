#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QSignalMapper>
#include <QObject>
#include <QMap>
#include <QVector>

#include "unit.h"

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"

class PnNetwork : public QGraphicsScene
{
  Q_OBJECT

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
  QVector<PnLine *> lineVector;

  bool addBar(PnBar *bar);
  bool addLine(PnLine *line);

  PnBar *getBarById(uint32_t id);
  PnLine *getLineByNodes(uint32_t noI, uint32_t noF);

signals:
  void barProperties(QObject *);
  void lineProperties(QObject *);

private:
  QSignalMapper *barDoubleClick;
  QSignalMapper *lineDoubleClick;
};

#endif  // PNNETWORK_H

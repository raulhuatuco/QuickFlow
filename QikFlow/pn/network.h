#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QSignalMapper>
#include <QObject>
#include <QMap>
#include <QVector>

#include "pn/types.h"

#include "pn/bar.h"
#include "pn/line.h"

QT_BEGIN_NAMESPACE
class PnBar;
class PnLine;
QT_END_NAMESPACE

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

  QMap<uint32_t, PnBar *> bars;
  QVector<PnLine *> lines;

  bool addBar(PnBar *bar);
  bool addLine(PnLine *line);

  PnBar *getBarById(uint32_t id);
  PnLine *getLineByNodes(uint32_t noI, uint32_t noF);

  void toPerUnit();
  void toBaseUnit();
  bool isPerUnit();

  void redrawAlgorithm1();

signals:
  void barProperties(QObject *);
  void lineProperties(QObject *);

private:
  QSignalMapper *barDoubleClick;
  QSignalMapper *lineDoubleClick;
  bool perUnit;

};

#endif  // PNNETWORK_H

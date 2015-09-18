#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QGraphicsObject>
#include <QSignalMapper>
#include <QObject>
#include <QMap>
#include <QVector>

#include "customtypes.h"

#include "pn/bar.h"
#include "pn/line.h"

QT_BEGIN_NAMESPACE
class Bar;
class Line;
QT_END_NAMESPACE

class Network : public QGraphicsScene
{
  Q_OBJECT

public:
  Network();
  ~Network();

  uint32_t maxIterations;
  double minError;

  double voltageBase;
  double powerBase;
  double currentBase;

  Unit::LengthUnit lengthUnit;
  Unit::ImpedanceUnit impedanceUnit;
  Unit::VoltageUnit voltageUnit;
  Unit::PowerUnit powerUnit;

  QMap<uint32_t, Bar *> bars;
  QVector<Line *> lines;

  bool addBar(Bar *bar);
  bool addLine(Line *line);

  Bar *getBarById(uint32_t id);
  Line *getLineByNodes(uint32_t noI, uint32_t noF);

  void toPerUnit();
  void toBaseUnit();
  bool isPerUnit();

signals:
  void barProperties(QObject *);
  void lineProperties(QObject *);

private:
  QSignalMapper *barDoubleClick;
  QSignalMapper *lineDoubleClick;
  bool perUnit;

};

#endif  // PNNETWORK_H

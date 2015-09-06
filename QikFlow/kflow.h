#ifndef KFLOW_H
#define KFLOW_H

#include <QString>
#include <QProcess>
#include <QObject>
#include <QWidget>

#include "PnGraphics/pnnetwork.h"

class Kflow : public QProcess {
 public:
  Kflow();
  Kflow(QWidget *parent);
  virtual ~Kflow();

  bool verbose;
  bool printIterations;

  double minError();
  bool setMinError(double minError);

  unsigned int maxIterations();
  bool setMaxIterations(unsigned int maxIterations);

  QString kFlowLocation();
  void setKflowLocation(QString kFlowLocation);

  bool generateInputFile(PnNetwork *pnNetwork);
  bool runSimulation();
  bool loadResults(PnNetwork *PnNetwork);

  double duration();
  unsigned int usedIterations();

 private:
  double minError_;
  unsigned int maxIterations_;
  double duration_;
  unsigned int usedIterations_;
};

#endif  // KFLOW_H

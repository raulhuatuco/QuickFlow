#ifndef KFLOW_H
#define KFLOW_H

#include <QString>
#include <QProcess>

#include "PnGraphics/pnnetwork.h"

class Kflow {
 public:
  Kflow();
  ~Kflow();

  bool verbose;
  bool printIterations;

  double minError();
  bool setMinError(double minError);

  unsigned int maxIterations();
  bool setMaxIterations(unsigned int maxIterations);

  QString workingDir();
  void setWorkingDir(QString workingDir);

  QString kFlowLocation();
  void setKflowLocation(QString kFlowLocation);

  bool generateInputFile(PnNetwork *pnNetwork);
  bool runSimulation();
  bool loadResults(PnNetwork *PnNetwork);

 private:
  double minError_;
  unsigned int maxIterations_;
  QString workingDir_;
  QString kFlowLocation_;

};

#endif  // KFLOW_H

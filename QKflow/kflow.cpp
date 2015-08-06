#include "kflow.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <complex>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pnline.h"
#include "PnGraphics/pncable.h"

Kflow::Kflow() : QProcess() {
  verbose = false;
  printIterations = false;
  minError_ = 0.0001;
  maxIterations_ = 1000;
}

Kflow::Kflow(QWidget *parent) : QProcess(parent) {
  verbose = false;
  printIterations = false;
  minError_ = 0.0001;
  maxIterations_ = 1000;
}

Kflow::~Kflow() {}

double Kflow::minError() { return minError_; }

bool Kflow::setMinError(double minError) {
  minError_ = minError;
  return true;
}

unsigned int Kflow::maxIterations() { return maxIterations_; }

bool Kflow::setMaxIterations(unsigned int maxIterations) {
  maxIterations_ = maxIterations;
  return true;
}

QString Kflow::kFlowLocation() { return program(); }

void Kflow::setKflowLocation(QString kFlowLocation) {
  setProgram(kFlowLocation);
}

bool Kflow::generateInputFile(PnNetwork *pnNetwork) {
  QFile inputFile(workingDirectory() + "/network.pnd");

  if (!inputFile.open(QIODevice::WriteOnly)) return false;

  QTextStream netData(&inputFile);

  foreach (PnBar *bar, pnNetwork->barMap) {
    if (bar->barType() == "Slack") {
      PnSlack *slack = dynamic_cast<PnSlack *>(bar);

      netData << "slack\t";

      netData << "(" << QString::number(slack->inputV().real()) << ","
              << QString::number(slack->inputV().imag()) << ")\t";

      netData << "(" << QString::number(slack->inputSl().real()) << ","
              << QString::number(slack->inputSl().imag()) << ")\t";

      netData << QString::number(slack->getMaxGeneration()) << endl;

    } else if (bar->barType() == "PQ") {
      PnPq *pq = dynamic_cast<PnPq *>(bar);

      netData << "pq\t";

      netData << QString::number(pq->Id()) << "\t";

      netData << "(" << QString::number(pq->outputSg().real()) << ","
              << QString::number(pq->outputSg().imag()) << ")\t";

      netData << "(" << QString::number(pq->inputSl().real()) << ","
              << QString::number(pq->inputSl().imag()) << ")";

      netData << endl;
    } else if (bar->barType() == "PV") {
      PnPv *pv = dynamic_cast<PnPv *>(bar);

      netData << "pv\t";

      netData << QString::number(pv->Id()) << "\t";

      netData << QString::number(pv->inputV().real()) << "\t";

      netData << QString::number(pv->outputSg().real()) << "\t";

      netData << "(" << QString::number(pv->inputSl().real()) << ","
              << QString::number(pv->inputSl().imag()) << ")\t";

      netData << QString::number(pv->getMaxQGenerated()) << "\t";

      netData << QString::number(pv->getMinQGenerated());

      netData << endl;
    }
  }

  foreach (PnLine *line, pnNetwork->lineMap) {
    if (line->lineType() == "Cable") {
      PnCable *cable = dynamic_cast<PnCable *>(line);

      netData << "line\t";
      netData << QString::number(cable->Id()) << "\t";
      netData << QString::number(cable->NoI()->Id()) << "\t";
      netData << QString::number(cable->NoF()->Id()) << "\t";
      netData << "(" << QString::number(cable->Impedance().real());
      netData << "," << QString::number(cable->Impedance().imag());
      netData << ")\t";
      netData << QString::number(cable->MaxLoad());
    }
  }

  netData.flush();
  inputFile.close();
  return true;
}

bool Kflow::runSimulation() {
  QStringList args;

  args << "-i";
  args << "network.pnd";
  args << "-o";
  args << "network.result";

  setArguments(args);

  start();
  return true;
}

bool Kflow::loadResults(PnNetwork *PnNetwork) {
  QFile outputFile(workingDirectory() + "/network.result");

  if (!outputFile.open(QIODevice::ReadOnly)) return false;

  QTextStream resultData(&outputFile);

  QString trash;

  resultData.readLine();
  resultData.readLine();
  resultData.readLine();

  unsigned int numBar;
  resultData >> trash >> trash >> trash >> numBar;
  unsigned int numLine;
  resultData >> trash >> trash >> trash >> numLine;

  resultData >> trash >> trash >> trash >> trash >> usedIterations_;
  resultData >> trash >> trash >> duration_;

  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();

  unsigned int barId;
  double absI;
  double argI;
  double absV;
  double argV;
  PnBar *bar;

  for (unsigned int i = 0; i < numBar; i++) {
    resultData >> trash >> barId;
    resultData >> trash >> absI;
    resultData >> trash >> argI;
    resultData >> trash >> absV;
    resultData >> trash >> argV;

    resultData.readLine();
    resultData.readLine();

    bar = PnNetwork->getBarById(barId);
    if (bar == NULL) continue;
    bar->setOutputV(std::polar(absV, argV * M_PI / 180.0));
    bar->setOutputI(std::polar(absI, argI * M_PI / 180.0));
  }

  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();
  resultData.readLine();

  unsigned int lineId;
  PnLine *line;
  for (unsigned int i = 0; i < numLine; i++) {
    resultData >> trash >> lineId;

    resultData >> trash >> absI;
    resultData >> trash >> argI;

    resultData.readLine();
    resultData.readLine();

    line = PnNetwork->getLineById(lineId);
    if (line == NULL) continue;

    line->setCurrent(std::polar(absI, argI * M_PI / 180.0));
  }

  return true;
}

double Kflow::duration() { return duration_; }

unsigned int Kflow::usedIterations() { return usedIterations_; }

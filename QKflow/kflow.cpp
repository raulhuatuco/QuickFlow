#include "kflow.h"

#include <QFile>
#include <QTextStream>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pnline.h"

Kflow::Kflow()
    : verbose(false),
      printIterations(false),
      minError_(0.0001),
      maxIterations_(1000),
      workingDir_(""),
      kFlowLocation_("kflow/kflow") {}

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

QString Kflow::workingDir() { return workingDir_; }

void Kflow::setWorkingDir(QString workingDir) { workingDir_ = workingDir; }

QString Kflow::kFlowLocation() { return kFlowLocation_; }

void Kflow::setKflowLocation(QString kFlowLocation) {
  kFlowLocation_ = kFlowLocation;
}

bool Kflow::generateInputFile(PnNetwork *pnNetwork) {
  QFile inputFile(workingDir_ + "/network.pdn");

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

  }

  netData.flush();
  inputFile.close();
  return true;
}

bool Kflow::runSimulation() { return true; }

bool Kflow::loadResults(PnNetwork *PnNetwork) { return true; }

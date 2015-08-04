#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>

#include "windowaddpv.h"
#include "windowaddslack.h"
#include "windowaddcable.h"

#include "kflow.h"

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"
#include "PnGraphics/pnnetwork.h"

QKflow::QKflow(QWidget *parent) : QMainWindow(parent), ui(new Ui::QKflow) {
  ui->setupUi(this);

  ui->actionPause->setDisabled(true);
  ui->actionStop->setDisabled(true);

  // Create network
  pnNetwork_ = new PnNetwork;
  ui->pnView->setPnNetwork(pnNetwork_);

  // Set background color
  QBrush bkbrush;
  bkbrush.setColor(QColor::fromRgbF(1.0, 1.0, 1.0, 1.0));
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);
}

QKflow::~QKflow() { delete ui; }

void QKflow::on_actionZoomIn_triggered() { ui->pnView->zoomIn(); }

void QKflow::on_actionZoomOut_triggered() { ui->pnView->zoomOut(); }

void QKflow::on_actionZoomFit_triggered() { ui->pnView->zoomFit(); }

void QKflow::on_actionSlack_triggered() {
  WindowAddSlack *windowAddSlack = new WindowAddSlack(this);
  windowAddSlack->setNetwork(pnNetwork_);
  windowAddSlack->exec();
  delete windowAddSlack;
}

void QKflow::on_actionPQBar_triggered() {
  WindowAddPq *windowAddPq = new WindowAddPq(this);
  windowAddPq->setNetwork(pnNetwork_);
  windowAddPq->exec();
  delete windowAddPq;
}

void QKflow::on_actionPVBar_triggered() {
  WindowAddPv *windowAddPv = new WindowAddPv(this);
  windowAddPv->setNetwork(pnNetwork_);
  windowAddPv->exec();
  delete windowAddPv;
}

void QKflow::on_actionCable_triggered() {
  WindowAddCable *windowAddCable = new WindowAddCable(this);
  windowAddCable->setNetwork(pnNetwork_);
  windowAddCable->exec();
  delete windowAddCable;
}

void QKflow::on_actionRun_triggered() {
  Kflow kflow;

  kflow.setWorkingDir("/home/akenge");
  kflow.generateInputFile(pnNetwork_);
}

#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>

#include "windowaddpq.h"
#include "windowaddpv.h"
#include "windowaddslack.h"
#include "windowaddcable.h"
#include "windowkflowsettings.h"

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"
#include "PnGraphics/pnnetwork.h"

QString const QKflow::kVersion = "0.0.1";

QKflow::QKflow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::QKflow), kflow(NULL) {
  ui->setupUi(this);

  // Adjust initial interface
  ui->actionCable->setDisabled(true);
  ui->actionPause->setDisabled(true);
  ui->actionPQBar->setDisabled(true);
  ui->actionPVBar->setDisabled(true);
  ui->actionRun->setDisabled(true);
  ui->actionSave->setDisabled(true);
  ui->actionSlack->setDisabled(true);
  ui->actionStop->setDisabled(true);
  ui->actionTransformer->setDisabled(true);
  ui->actionZoomFit->setDisabled(true);
  ui->actionZoomIn->setDisabled(true);
  ui->actionZoomOut->setDisabled(true);
  ui->pnView->setDisabled(true);

  // Set application info
  QCoreApplication::setOrganizationName("DKrepsky");
  QCoreApplication::setOrganizationDomain("dkrepsky.blogspot.com.br");
  QCoreApplication::setApplicationName("QkFlow");
  QCoreApplication::setApplicationVersion(kVersion);

  // Create base objects
  settings = new QSettings(this);
  kflow = new Kflow(this);

  // if settings exists, use it. Otherwise create a new one.
  if (settings->contains("version")) {
    if (settings->value("version").toString() == kVersion)
      loadSettings();
    else
      upgradeSettings();
  } else {
    createSettings();
  }
}

QKflow::~QKflow() {
  saveSettings();
  delete ui;
}

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
  if (kflow == NULL) return;

  kflow->generateInputFile(pnNetwork_);
  kflow->runSimulation();

  while (!kflow->waitForFinished()) {
  }

  if (kflow->error() == QProcess::Crashed) {
    QMessageBox::critical(this, tr("Simulation Error: ") + kflow->errorString(),
                          kflow->readAll(), QMessageBox::Ok);
    return;
  } else {
    kflow->loadResults(pnNetwork_);
    QMessageBox::information(
        this, tr("Simulation duration"),
        "Simulation time: " + QString::number(kflow->duration()) + "s",
        QMessageBox::Ok);
  }
}

void QKflow::on_actionSettings_triggered() {
  WindowKflowSettings *windowKflowSettings = new WindowKflowSettings(this);
  windowKflowSettings->setKflow(kflow);
  windowKflowSettings->exec();
  delete windowKflowSettings;
  saveSettings();
}

void QKflow::on_actionNew_triggered() {
  // Create network
  pnNetwork_ = new PnNetwork;
  ui->pnView->setPnNetwork(pnNetwork_);
}

void QKflow::on_actionOpen_triggered() {}

void QKflow::loadSettings() {
  // kFlow Settings
  QString kflowPath = settings->value("kflow_path").toString();
  kflow->setKflowLocation(kflowPath);
  kflow->setMaxIterations(settings->value("kflow_maxIterations").toUInt());
  kflow->setMinError(settings->value("kflow_minError").toDouble());
  kflow->verbose = settings->value("kflow_verbose").toBool();
  kflow->printIterations = settings->value("kflow_printIterations").toBool();

  // pnView Settings
  QBrush bkbrush;
  bkbrush.setColor(settings->value("pnView_background").value<QColor>());
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);
}

void QKflow::saveSettings() {
  // kFlow Settings
  settings->setValue("kflow_path", kflow->kFlowLocation());
  settings->setValue("kflow_maxIterations", kflow->maxIterations());
  settings->setValue("kflow_minError", kflow->minError());
  settings->setValue("kflow_verbose", kflow->verbose);
  settings->setValue("kflow_printIterations", kflow->printIterations);

  // pnView Settings
  settings->setValue("pnView_background", ui->pnView->backgroundBrush());
  settings->sync();
}

void QKflow::createSettings() {
  // Save version
  settings->setValue("version", kVersion);

  // kFlow Settings
  if (QSysInfo::productType() == "windows")
    kflow->setKflowLocation("kflow/KFlow.exe");
  else
    kflow->setKflowLocation("kflow/KFlow");

  kflow->setMaxIterations(1000);
  kflow->setMinError(0.00001);
  kflow->verbose = false;
  kflow->printIterations = false;

  // pnView Settings
  QBrush bkbrush;
  bkbrush.setColor(Qt::white);
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);

  saveSettings();
}

void QKflow::upgradeSettings() {}

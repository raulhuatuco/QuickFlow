#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

#include "windowaddpq.h"
#include "windowaddpv.h"
#include "windowaddslack.h"
#include "windowaddcable.h"
#include "windowkflowsettings.h"
#include "windownewproject.h"

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
  noProjectInterface();

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

  // Project has not beed altered
  altered_ = false;
  projectSettings_ = NULL;
}

QKflow::~QKflow() {
  delete ui;
}

bool QKflow::isAltered() {
  return altered_;
}

void QKflow::setAltered(bool altered) {
  altered_ = altered;

  if(altered_)
    ui->actionSave->setEnabled(true);
}

void QKflow::noProjectInterface() {
  ui->actionCable->setDisabled(true);
  ui->actionClose->setDisabled(true);
  ui->actionPause->setDisabled(true);
  ui->actionPQBar->setDisabled(true);
  ui->actionPVBar->setDisabled(true);
  ui->actionRun->setDisabled(true);
  ui->actionSave->setDisabled(true);
  ui->actionSave_as->setDisabled(true);
  ui->actionSlack->setDisabled(true);
  ui->actionStop->setDisabled(true);
  ui->actionTransformer->setDisabled(true);
  ui->actionZoomFit->setDisabled(true);
  ui->actionZoomIn->setDisabled(true);
  ui->actionZoomOut->setDisabled(true);
  ui->pnView->setDisabled(true);
}

void QKflow::workInterface() {
  ui->actionCable->setDisabled(false);
  ui->actionClose->setDisabled(false);
  ui->actionPQBar->setDisabled(false);
  ui->actionPVBar->setDisabled(false);
  ui->actionRun->setDisabled(false);
  ui->actionSave_as->setDisabled(false);
  ui->actionSlack->setDisabled(false);
  ui->actionTransformer->setDisabled(false);
  ui->actionZoomFit->setDisabled(false);
  ui->actionZoomIn->setDisabled(false);
  ui->actionZoomOut->setDisabled(false);
  ui->pnView->setDisabled(false);
}

void QKflow::on_actionZoomIn_triggered() {
  ui->pnView->zoomIn();
}

void QKflow::on_actionZoomOut_triggered() {
  ui->pnView->zoomOut();
}

void QKflow::on_actionZoomFit_triggered() {
  ui->pnView->zoomFit();
}

void QKflow::on_actionSlack_triggered() {
  WindowAddSlack *windowAddSlack = new WindowAddSlack(this);
  windowAddSlack->setNetwork(projectSettings_->pnNetwork());

  if(windowAddSlack->exec() == QDialog::Accepted)
    setAltered(true);

  delete windowAddSlack;
}

void QKflow::on_actionPQBar_triggered() {
  WindowAddPq *windowAddPq = new WindowAddPq(this);
  windowAddPq->setNetwork(projectSettings_->pnNetwork());

  if(windowAddPq->exec() == QDialog::Accepted)
    setAltered(true);

  delete windowAddPq;
}

void QKflow::on_actionPVBar_triggered() {
  WindowAddPv *windowAddPv = new WindowAddPv(this);
  windowAddPv->setNetwork(projectSettings_->pnNetwork());

  if(windowAddPv->exec() == QDialog::Accepted)
    setAltered(true);

  delete windowAddPv;
}

void QKflow::on_actionCable_triggered() {
  WindowAddCable *windowAddCable = new WindowAddCable(this);
  windowAddCable->setNetwork(projectSettings_->pnNetwork());

  if(windowAddCable->exec() == QDialog::Accepted)
    setAltered(true);

  delete windowAddCable;
}

void QKflow::on_actionRun_triggered() {
  if (kflow == NULL) return;

  kflow->generateInputFile(projectSettings_->pnNetwork());
  kflow->runSimulation();

  while (!kflow->waitForFinished()) {
  }

  if (kflow->error() == QProcess::Crashed) {
    QMessageBox::critical(this, tr("Simulation Error: ") + kflow->errorString(),
                          kflow->readAll(), QMessageBox::Ok);
    return;
  } else {
    kflow->loadResults(projectSettings_->pnNetwork());
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
  // Check if there is a project already opened
  if (projectSettings_ != NULL) {
    // Close project
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (projectSettings_ != NULL) return;
  }

  // Open New Project window.
  WindowNewProject *windowNewProject = new WindowNewProject(this);

  // If canceled return
  // windowNewProject MUST provide valid data
  if (windowNewProject->exec() != QDialog::Accepted) {
    delete windowNewProject;
    return;
  }

  // Create project object
  projectSettings_ = new ProjectSettings;
  projectSettings_->setName(windowNewProject->name());

  // Try to save project settings
  bool save_ok =
    projectSettings_->save(windowNewProject->path() + QDir::separator() +
                           windowNewProject->name() + ".qkflow");

  if (!save_ok) {
    QMessageBox::critical(this, "File write error",
                          "Cannot write .qkflow file.", QMessageBox::Ok);
    delete projectSettings_;
    projectSettings_ = NULL;
    delete windowNewProject;
    return;
  }

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + windowNewProject->name());

  // Set working directory
  kflow->setWorkingDirectory(windowNewProject->path());

  // Enable interface
  workInterface();

  // Set screne
  ui->pnView->setPnNetwork(projectSettings_->pnNetwork());

  // Delete window
  delete windowNewProject;
}

void QKflow::on_actionOpen_triggered() {
  // Check if there is a project already opened
  if (projectSettings_ != NULL) {
    // Close project
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (projectSettings_ != NULL) return;
  }

  // Open file dialog to select the project file.
  QFileDialog project(this);
  project.setFileMode(QFileDialog::ExistingFile);
  project.setAcceptMode(QFileDialog::AcceptOpen);
  project.setNameFilter(tr("QkFlow Project (*.qkflow)"));
  project.setDirectory(QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening
  if (project.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename
  QString fileName = project.selectedFiles()[0];

  // Create project object
  projectSettings_ = new ProjectSettings;

  if (projectSettings_->load(fileName) != true) {
    QMessageBox::critical(this, "File read error", "Cannot read .qkflow file.",
                          QMessageBox::Ok);
    delete projectSettings_;
    return;
  }

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + projectSettings_->name());

  // Set working directory
  QDir workingDirectory = QFileInfo(fileName).absoluteDir();
  kflow->setWorkingDirectory(workingDirectory.path());

  // Enable interface
  workInterface();

  // Set screne
  ui->pnView->setPnNetwork(projectSettings_->pnNetwork());
}

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
    kflow->setKflowLocation(qApp->applicationDirPath() + QDir::separator() +
                            "kflow/KFlow.exe");
  else
    kflow->setKflowLocation(qApp->applicationDirPath() + QDir::separator() +
                            "kflow/KFlow");

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

void QKflow::on_actionSave_triggered() {
  // Check for alterations
  if (altered_) {
    // If altered, save.
    QString fileName = kflow->workingDirectory() + QDir::separator() +
                       projectSettings_->name() + ".qkflow";

    if (projectSettings_->save(fileName) != true) {
      QMessageBox::critical(this, "Write to file failed",
                            "Cannot save project.", QMessageBox::Ok);
      return;
    }
  }

  // Disable save button
  ui->actionSave->setEnabled(false);
  altered_ = false;
}

void QKflow::on_actionSave_as_triggered() {
  // Get file location
  QFileDialog directory(this);
  directory.setFileMode(QFileDialog::Directory);
  directory.setOption(QFileDialog::ShowDirsOnly);
  directory.setAcceptMode(QFileDialog::AcceptSave);
  directory.setWindowTitle("Project path");

  if (directory.exec() != QDialog::Accepted) return;

  QString newdir = directory.selectedFiles()[0];

  // Try to save
  // On error, return
  if (projectSettings_->save(newdir + QDir::separator() +
                             projectSettings_->name() + tr(".qkflow")) !=
      true) {
    QMessageBox::critical(this, "Write to file failed", "Cannot save project.",
                          QMessageBox::Ok);
    return;
  }

  // Save is ok, set the new path
  kflow->setWorkingDirectory(newdir);

  // Disable save
  altered_ = false;
  ui->actionSave->setEnabled(false);
}

void QKflow::on_actionClose_triggered() {
  // Check if project is open
  if (projectSettings_ == NULL) {
    // Disable interface and return
    noProjectInterface();
    return;
  }

  // Check whether it does need to save the project or not
  if (altered_) {
    // Ask user to discard changes
    int button = QMessageBox::question(
                   this, "Save", "There are unsaved changes, save it?",
                   QMessageBox::Discard | QMessageBox::Cancel | QMessageBox::Save);

    // User wants to save
    if (button == QMessageBox::Save) {
      // Try to save
      ui->actionSave->trigger();

      // Check if save is Ok
      if (altered_) {
        // User canceled
        return;
      }
    } else if (button == QMessageBox::Cancel) {
      // User canceled
      return;
    }
  }

  delete projectSettings_;
  projectSettings_ = NULL;
  kflow->setWorkingDirectory(tr(""));
  altered_ = false;
  noProjectInterface();
}

// Hook to application exit
void QKflow::closeEvent(QCloseEvent *event) {
  // Check for unsaved changes
  if (altered_) {
    // Try to close project, saving the changes
    ui->actionClose->trigger();

    // Check for a  cancel
    if (altered_) {
      // User doesnt want to quit
      event->ignore();
      return;
    }
  }

  saveSettings();
  event->accept();
}

void QKflow::on_actionExit_triggered() {
  close();
}

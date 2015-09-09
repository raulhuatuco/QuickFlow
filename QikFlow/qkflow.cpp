#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"
#include "PnGraphics/pnnetwork.h"

#include "window/newproject.h"
#include "window/barproperties.h"
#include "window/lineproperties.h"

QString const kVersion = "0.0.3";

/*******************************************************************************
 * QKflow
 ******************************************************************************/
QKflow::QKflow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::QKflow),
    project(NULL),
    altered_ (false) {
  // Set-up user interface.
  ui->setupUi(this);

  // Adjust initial interface to disable project related actions.
  noProjectInterface();

  // Set application info.
  QCoreApplication::setOrganizationName("DKrepsky");
  QCoreApplication::setOrganizationDomain("dkrepsky.blogspot.com.br");
  QCoreApplication::setApplicationName("QkFlow");
  QCoreApplication::setApplicationVersion(kVersion);

  // Check for settings existence and version.
  settings = new QSettings(this);

  if (settings->contains("version")) {
    if (settings->value("version").toString() == kVersion)
      // If exists and is in the current version, use it.
      loadSettings();
    else
      // Update to the new program version.
      upgradeSettings();
  } else {
    // If there is no settings availiable, create a new one with default values.
    createSettings();
  }
}

/*******************************************************************************
 * ~QKflow
 ******************************************************************************/
QKflow::~QKflow() {
  delete ui;
}

/*******************************************************************************
 * isAltered
 ******************************************************************************/
bool QKflow::isAltered() {
  return altered_;
}

/*******************************************************************************
 * setAltered
 ******************************************************************************/
void QKflow::setAltered(bool altered) {
  altered_ = altered;

  if(altered_)
    ui->actionSave->setEnabled(true);
}

/*******************************************************************************
 * noProjectInterface
 ******************************************************************************/
void QKflow::noProjectInterface() {
  // Project actions.
  ui->actionNew->setEnabled(true);
  ui->actionOpen->setEnabled(true);
  ui->actionSave->setEnabled(false);
  ui->actionSave_as->setEnabled(false);
  ui->actionClose->setEnabled(false);
  ui->actionSettings->setEnabled(false);

  // Simulation actions.
  ui->actionPause->setEnabled(false);
  ui->actionRun->setEnabled(false);
  ui->actionStop->setEnabled(false);

  // Zoom actions.
  ui->actionZoomFit->setEnabled(false);
  ui->actionZoomIn->setEnabled(false);
  ui->actionZoomOut->setEnabled(false);

  // Power network actions
  ui->actionAddBar->setEnabled(false);
  ui->actionAddLine->setEnabled(false);

  // Disable viewer
//  ui->pnView->setEnabled(false);
}

/*******************************************************************************
 * workInterface
 ******************************************************************************/
void QKflow::workInterface() {
  // Project actions.
  ui->actionNew->setEnabled(true);
  ui->actionOpen->setEnabled(true);
  ui->actionSave->setEnabled(false);
  ui->actionSave_as->setEnabled(true);
  ui->actionClose->setEnabled(true);
  ui->actionSettings->setEnabled(true);

  // Simulation actions.
  ui->actionPause->setEnabled(true);
  ui->actionRun->setEnabled(true);
  ui->actionStop->setEnabled(true);

  // Zoom actions.
  ui->actionZoomFit->setEnabled(true);
  ui->actionZoomIn->setEnabled(true);
  ui->actionZoomOut->setEnabled(true);

  // Power network actions
  ui->actionAddBar->setEnabled(true);
  ui->actionAddLine->setEnabled(true);

  // Enable viewer
//  ui->pnView->setEnabled(true);
}

/*******************************************************************************
 * loadSettings
 ******************************************************************************/
void QKflow::loadSettings() {
  // pnView background.
  QBrush bkbrush;
  bkbrush.setColor(settings->value("pnView_background").value<QColor>());
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);
}

/*******************************************************************************
 * saveSettings
 ******************************************************************************/
void QKflow::saveSettings() {
  // pnView background.
  settings->setValue("pnView_background", ui->pnView->backgroundBrush());

  // Save settings.
  settings->sync();
}

/*******************************************************************************
 * createSettings
 ******************************************************************************/
void QKflow::createSettings() {
  // Save version.
  settings->setValue("version", kVersion);

//  pnView background.
  QBrush bkbrush;
  bkbrush.setColor(Qt::white);
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);

  // Store settings.
  saveSettings();
}

/*******************************************************************************
 * upgradeSettings
 ******************************************************************************/
void QKflow::upgradeSettings() {
  // Remove everything and create a default.
  settings->clear();
  createSettings();
}

/*******************************************************************************
 * Action New Project triggered.
 ******************************************************************************/
void QKflow::on_actionNew_triggered() {
  // Check if there is a project already opened.
  if (project != NULL) {
    // Close project
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (project != NULL) return;
  }

  // Open New Project window and gather the project settings.
  // NOTE: windowNewProject MUST provide valid data.
  NewProject *newProject = new NewProject(this);

  // If canceled return
  if (newProject->exec() != QDialog::Accepted) {
    delete newProject;
    return;
  }

  // Create project object and fill settings.
  project = new Project;
  project->name = newProject->dataName;
  project->filepath = newProject->dataPath + QDir::separator() +
                      newProject->dataName + ".qkflow";

  // Simulation data.
  project->setMaxIterations(newProject->dataMaxIterations);
  project->setMinError(newProject->dataMinError);
  project->setVoltageBase(newProject->dataVoltageBase);
  project->setPowerBase(newProject->dataPowerBase);

  // Units.
  project->setLengthUn(
    newProject->dataLengthUnit);

  project->setImpedanceUn(
    newProject->dataImpedanceUnit);
  project->setVoltageUn(
    newProject->dataVoltageUnit);
  project->setPowerUn(
    newProject->dataPowerUnit);

  // Try to save project.
  bool save_ok =
    project->save();

  if (!save_ok) {
    QMessageBox::critical(this, "File write error",
                          "Cannot write .qkflow file.", QMessageBox::Ok);
    delete project;
    project = NULL;
    delete newProject;
    return;
  }

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + newProject->dataName);
  // Enable interface
  workInterface();
  // Set screne
  ui->pnView->setPnNetwork(project->pnNetwork);

  // Delete window
  delete newProject;
}

/*******************************************************************************
 * Action Open triggered.
 ******************************************************************************/
void QKflow::on_actionOpen_triggered() {
  // Check if there is a project already opened.
  if (project != NULL) {
    // Close project.
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (project != NULL) return;
  }

  // Open file dialog to select the project file.
  QFileDialog projectFile(this);
  projectFile.setFileMode(QFileDialog::ExistingFile);
  projectFile.setAcceptMode(QFileDialog::AcceptOpen);
  projectFile.setNameFilter(tr("QkFlow Project (*.qkflow)"));
  projectFile.setDirectory(QStandardPaths::standardLocations(
                             QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (projectFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString fileName = projectFile.selectedFiles()[0];

  // Create project object and load from file.
  project = new Project;
  project->filepath = fileName;

  if (project->load() != true) {
    QMessageBox::critical(this, "File read error", "Cannot read .qkflow file.",
                          QMessageBox::Ok);
    delete project;
    project = NULL;
    return;
  }

  // New project is Ok.
  // Change window title to indicate that the project is open.
  setWindowTitle("QkFlow - " + project->name);
  // Enable interface.
  workInterface();

  // Set screne.
  ui->pnView->setPnNetwork(project->pnNetwork);
}

/*******************************************************************************
 * Action Save triggered.
 ******************************************************************************/
void QKflow::on_actionSave_triggered() {
  // Check for alterations
  if (altered_) {
    if (project->save() != true) {
      QMessageBox::critical(this, "Write to file failed",
                            "Cannot save project.", QMessageBox::Ok);
      return;
    }
  }

  // Project doesnt have any new modifications now.
  // Disable save button.
  ui->actionSave->setEnabled(false);
  altered_ = false;
}

/*******************************************************************************
 * Action Save As triggered.
 ******************************************************************************/
void QKflow::on_actionSave_as_triggered() {
  // Get file location from user.
  QFileDialog projectFile(this);
  projectFile.setAcceptMode(QFileDialog::AcceptSave);
  projectFile.setNameFilter(tr("QkFlow Project (*.qkflow)"));
  projectFile.setDirectory(QStandardPaths::standardLocations(
                             QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (projectFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString filePath = projectFile.selectedFiles()[0];

  // Try to save.
  if (project->saveAs(filePath) != true) {
    QMessageBox::critical(this, "Write to file failed",
                          "Cannot save project.", QMessageBox::Ok);
    return;
  }

  // Adjust Project settings.
  project->filepath = filePath;

  // Project doesnt have any new modifications now.
  // Disable save button.
  ui->actionSave->setEnabled(false);
  altered_ = false;
}

/*******************************************************************************
* Action Close triggered.
******************************************************************************/
void QKflow::on_actionClose_triggered() {
  // Check if project is open
  if (project == NULL) {
    // Disable interface and return
    noProjectInterface();
    return;
  }

  // A project is open. Check for alterations and try to save.
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

  delete project;
  project = NULL;
  altered_ = false;
  noProjectInterface();
}

/*******************************************************************************
 * Action ZoomIn triggered.
 ******************************************************************************/
void QKflow::on_actionZoomIn_triggered() {
  ui->pnView->zoomIn();
}

/*******************************************************************************
 * Action ZoomOut triggered.
 ******************************************************************************/
void QKflow::on_actionZoomOut_triggered() {
  ui->pnView->zoomOut();
}

/*******************************************************************************
 * Action ZoomFit triggered.
 ******************************************************************************/
void QKflow::on_actionZoomFit_triggered() {
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Add Bar triggered.
 ******************************************************************************/
void QKflow::on_actionAddBar_triggered() {
  // Create a new bar that will be used by bar properties window.
  PnBar *bar = new PnBar;

  BarProperties *barProperties = new BarProperties(this);
  barProperties->setBar(bar, true);
  barProperties->setUnit(project->pnNetwork->voltageUnit,
                         project->pnNetwork->powerUnit);

  if (barProperties->exec() == QDialog::Accepted) {
    setAltered(true);
    project->pnNetwork->addBar(bar);
  } else {
    delete bar;
  }

  delete barProperties;
}

/*******************************************************************************
 * Action Add Line triggered.
 ******************************************************************************/
void QKflow::on_actionAddLine_triggered() {
  PnLine *line = new PnLine;

  LineProperties lineProperties(this);
  lineProperties.setLine(line, true);
  lineProperties.setBarMap(project->pnNetwork->barMap);
  lineProperties.setUnit(project->lengthUn(), project->impedanceUn());

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
    project->pnNetwork->addLine(line);
  } else {
    delete line;
  }
}

/*******************************************************************************
 * Action Exit triggered.
 ******************************************************************************/
void QKflow::on_actionExit_triggered() {
  close();
}

/*******************************************************************************
 * closeEvent.
 ******************************************************************************/
void QKflow::closeEvent(QCloseEvent *event) {
  // Hook to application exit.
  // Check for unsaved changes.
  if (altered_) {
    // Try to close project, saving the changes.
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

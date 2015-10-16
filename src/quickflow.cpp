/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \addtogroup Window
 * \{
 */

/*!
 * \file quickflow.cpp
 *
 * \brief Main window class implementation.
 *
 * This file is the implementation of the QuickFlow class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#include "quickflow.h"
#include "ui_quickflow.h"


#include "window/newproject.h"
#include "window/networkproperties.h"
#include "window/barproperties.h"
#include "window/lineproperties.h"
#include "window/about.h"

#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

#include "algorithms/import.h"
#include "algorithms/redraw.h"
#include "algorithms/shirmoharmmadi.h"

/*****************************************************************************
* Const.
*****************************************************************************/
const QString QuickFlow::kVersion = "0.0.5";

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
QuickFlow::QuickFlow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::QuickFlow),
    project(NULL),
    altered_ (false)
{
  // Set-up user interface.
  ui->setupUi(this);

  // Adjust initial interface to disable project related actions.
  noProjectInterface();

  // Set application info.
  QCoreApplication::setOrganizationName("DKrepsky");
  QCoreApplication::setOrganizationDomain("dkrepsky.blogspot.com.br");
  QCoreApplication::setApplicationName("QuickFlow");
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

  connectSignals();
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
QuickFlow::~QuickFlow()
{
  disconnectSignals();
  delete ui;
}

/*******************************************************************************
 * Is altered.
 ******************************************************************************/
bool QuickFlow::isAltered()
{
  return altered_;
}

/*******************************************************************************
 * Set altered.
 ******************************************************************************/
void QuickFlow::setAltered(bool altered)
{
  altered_ = altered;

  if(altered_) {
    ui->actionSave->setEnabled(true);
    // Change window title to indicate that the project is altered.
    setWindowTitle("QkFlow - *" + project->name);
  } else {
    ui->actionSave->setEnabled(false);
    // Change window title to indicate that the project has been saved.
    setWindowTitle("QkFlow - " + project->name);
  }
}

/*******************************************************************************
 * Zoom in action.
 ******************************************************************************/
void QuickFlow::on_actionZoomIn_triggered()
{
  ui->systemView->zoomIn();
}

/*******************************************************************************
 * Zoom out action.
 ******************************************************************************/
void QuickFlow::on_actionZoomOut_triggered()
{
  ui->systemView->zoomOut();
}

/*******************************************************************************
 * Zoom fit action.
 ******************************************************************************/
void QuickFlow::on_actionZoomFit_triggered()
{
  ui->systemView->zoomFit();
}

/*******************************************************************************
 * Exit action.
 ******************************************************************************/
void QuickFlow::on_actionExit_triggered()
{
  close();
}

/*******************************************************************************
 * New project action.
 ******************************************************************************/
void QuickFlow::on_actionNew_triggered()
{
  // Check if there is a project already opened.
  if (project != NULL) {
    // Close project
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (project != NULL) return;
  }

  // Open New Project window and gather the project settings.
  // NOTE: windowNewProject MUST provide valid data.
  NewProject newProject(this);

  // If canceled return
  if (newProject.exec() != QDialog::Accepted) {
    return;
  }

  // Create project object and fill settings.
  project = new Project;
  project->name = newProject.dataName;
  project->filePath = newProject.dataPath + QDir::separator() +
                      newProject.dataName + ".qkf";

  // Simulation data.
  project->setMaxIterations(newProject.dataMaxIterations);
  project->setMinError(newProject.dataMinError);
  project->setVoltageBase(newProject.dataVoltageBase);
  project->setPowerBase(newProject.dataPowerBase);

  // Units.
  project->setLengthUn(newProject.dataLengthUnit);
  project->setImpedanceUn(newProject.dataImpedanceUnit);
  project->setVoltageUn(newProject.dataVoltageUnit);
  project->setPowerUn(newProject.dataPowerUnit);
  project->setCurrentUnit(newProject.dataCurrentUnit);

  // Try to save project.
  bool save_ok = project->save();

  if (!save_ok) {
    QMessageBox::critical(this, "File write error",
                          "Cannot write .qkf file.", QMessageBox::Ok);
    delete project;
    project = NULL;
    return;
  }

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + newProject.dataName);
  // Enable interface
  workInterface();
}

/*******************************************************************************
 * Open project action.
 ******************************************************************************/
void QuickFlow::on_actionOpen_triggered()
{
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
  projectFile.setNameFilter(tr("QkFlow Project (*.qkf)"));
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
  project->filePath = fileName;

  if (project->load() != true) {
    QMessageBox::critical(this, "File read error", "Cannot read .qkf file.",
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

  // Add networks
  foreach(Network *network, project->networks) {
    ui->systemView->addNetwork(network);
  }
}

/*******************************************************************************
 * Save project action.
 ******************************************************************************/
void QuickFlow::on_actionSave_triggered()
{
  // Check for alterations
  if (altered_) {
    if (project->save() != true) {
      QMessageBox::critical(this, "Write to file failed",
                            "Cannot save project.", QMessageBox::Ok);
      return;
    }
  }

  setAltered(false);
}

/*******************************************************************************
 * Save project as action.
 ******************************************************************************/
void QuickFlow::on_actionSave_as_triggered()
{
  // Get file location from user.
  QFileDialog projectFile(this);
  projectFile.setAcceptMode(QFileDialog::AcceptSave);
  projectFile.setNameFilter(tr("QkFlow Project (*.qkf)"));
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
  project->filePath = filePath;

  setAltered(false);
}

/*******************************************************************************
* Close project action.
******************************************************************************/
void QuickFlow::on_actionClose_triggered()
{
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

  foreach(Network *network, project->networks) {
    ui->systemView->removeNetwork(network);
  }

  ui->systemView->zoomFit();

  delete project;
  project = NULL;
  altered_ = false;
  noProjectInterface();
  setWindowTitle("QuickFlow");
}


/*******************************************************************************
* Add network action.
******************************************************************************/
void QuickFlow::on_actionAdd_Network_triggered()
{
  NetworkProperties newNetwork(this);
  newNetwork.setOptions(project, NULL);

  if (newNetwork.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Add bar action.
 ******************************************************************************/
void QuickFlow::on_actionAddBar_triggered()
{
  BarProperties barProperties(this);
  barProperties.setOptions(project, NULL);

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);
    ui->systemView->addBar(barProperties.bar());
  }
}

/*******************************************************************************
 * Add line action.
 ******************************************************************************/
void QuickFlow::on_actionAddLine_triggered()
{
  LineProperties lineProperties(this);
  lineProperties.setOptions(project, NULL);

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
    ui->systemView->addLine(lineProperties.line());
  }
}

/*******************************************************************************
 * Import txt type 1 action.
 ******************************************************************************/
void QuickFlow::on_action_txt_type_1_triggered()
{
  // Get file location from user.
  QFileDialog txtFile(this);
  txtFile.setAcceptMode(QFileDialog::AcceptOpen);
  txtFile.setFileMode(QFileDialog::ExistingFile);
  txtFile.setNameFilter(tr("Text File Type 1(*.txt)"));
  txtFile.setDirectory(QStandardPaths::standardLocations(
                         QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (txtFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString fileName = txtFile.selectedFiles()[0];

  // Check if there is a project already opened.
  if (project == NULL) {
    // Create a new project.
    on_actionNew_triggered();

    // Check if project has been created.
    if(project == NULL) {
      // User canceled, cancel import.
      QMessageBox::critical(this, "Need a project to import",
                            "A project is needed in order to import networks.",
                            QMessageBox::Ok);
      return;
    }
  }

  NetworkProperties newNetwork(this);
  newNetwork.setOptions(project, NULL);

  if (newNetwork.exec() == QDialog::Rejected) {
    // User canceled import, return.
    return;
  }

  // Try to import network.
  Network *network = newNetwork.network();
  bool importOk = importTxtType1(fileName, network);

  // Impor success.
  if(importOk) {
    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    project->networks.remove(network->name);
    delete network;
  }
}

/*******************************************************************************
 * Import txt type 2 action.
 ******************************************************************************/
void QuickFlow::on_action_txt_type_2_triggered()
{
  // Get file location from user.
  QFileDialog txtFile(this);
  txtFile.setAcceptMode(QFileDialog::AcceptOpen);
  txtFile.setFileMode(QFileDialog::ExistingFile);
  txtFile.setNameFilter(tr("Text File Type 1(*.txt)"));
  txtFile.setDirectory(QStandardPaths::standardLocations(
                         QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (txtFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString fileName = txtFile.selectedFiles()[0];

  // Check if there is a project already opened.
  if (project == NULL) {
    // Create a new project.
    on_actionNew_triggered();

    // Check if project has been created.
    if(project == NULL) {
      // User canceled, cancel import.
      QMessageBox::critical(this, "Need a project to import",
                            "A project is needed in order to import networks.",
                            QMessageBox::Ok);
      return;
    }
  }

  NetworkProperties newNetwork(this);
  newNetwork.setOptions(project, NULL);

  if (newNetwork.exec() == QDialog::Rejected) {
    // User canceled import, return.
    return;
  }

  // Try to import network.
  Network *network = newNetwork.network();
  bool importOk = importTxtType2(fileName, network);

  // Impor success.
  if(importOk) {
    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    project->networks.remove(network->name);
    delete network;
  }
}

/*******************************************************************************
 * Import txt type 3 action.
 ******************************************************************************/
void QuickFlow::on_action_txt_type_3_triggered()
{
  // Get file location from user.
  QFileDialog txtFile(this);
  txtFile.setAcceptMode(QFileDialog::AcceptOpen);
  txtFile.setFileMode(QFileDialog::ExistingFile);
  txtFile.setNameFilter(tr("Text File Type 1(*.txt)"));
  txtFile.setDirectory(QStandardPaths::standardLocations(
                         QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (txtFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString fileName = txtFile.selectedFiles()[0];

  // Check if there is a project already opened.
  if (project == NULL) {
    // Create a new project.
    on_actionNew_triggered();

    // Check if project has been created.
    if(project == NULL) {
      // User canceled, cancel import.
      QMessageBox::critical(this, "Need a project to import",
                            "A project is needed in order to import networks.",
                            QMessageBox::Ok);
      return;
    }
  }

  NetworkProperties newNetwork(this);
  newNetwork.setOptions(project, NULL);

  if (newNetwork.exec() == QDialog::Rejected) {
    // User canceled import, return.
    return;
  }

  // Try to import network.
  Network *network = newNetwork.network();
  bool importOk = importTxtType3(fileName, network);

  // Impor success.
  if(importOk) {
    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    project->networks.remove(network->name);
    delete network;
  }
}

/*******************************************************************************
 * Cespedes action.
 ******************************************************************************/
void QuickFlow::on_actionCespedes_triggered()
{
  if(ui->actionShirmoharmmadi->isChecked()) {
    ui->actionShirmoharmmadi->setChecked(false);
  }
}

/*******************************************************************************
 * Shirmoharmmadi action.
 ******************************************************************************/
void QuickFlow::on_actionShirmoharmmadi_triggered()
{
  if(ui->actionCespedes->isChecked()) {
    ui->actionCespedes->setChecked(false);
  }
}

/*******************************************************************************
 * Sugiyama graph algorithm action.
 ******************************************************************************/
void QuickFlow::on_actionSugiyama_triggered()
{

  foreach(Network *network, project->networks) {
    redrawSugiyama(network);
  }

  setAltered(true);
  ui->systemView->zoomFit();
}

/*******************************************************************************
 * Sugiyama fast graph algorithm action.
 ******************************************************************************/
void QuickFlow::on_actionSugiyama_Fast_triggered()
{
  foreach(Network *network, project->networks) {
    redrawSugiyamaFast(network);
  }

  setAltered(true);
  ui->systemView->zoomFit();
}

/*******************************************************************************
 *  Multi-level graph algorithm action.
 ******************************************************************************/
void QuickFlow::on_actionMulti_level_triggered()
{
  foreach(Network *network, project->networks) {
    redrawMultiLevel(network);
  }

  setAltered(true);
  ui->systemView->zoomFit();
}

/*******************************************************************************
 * Search bar action.
 ******************************************************************************/
void QuickFlow::on_actionSearch_Bar_triggered()
{
//  bool ok;
//  int barId = QInputDialog::getInt(this, tr("Search for bar"), tr("Bad Id"), 0, 0,
//                                   1000000, 1, &ok);

//  if(ok) {
    //Bar *bar = project->network->getBarById(barId);

    //if (bar != NULL) {
//      ui->systemView->fitInView(bar->x() -100, bar->y() -100,
//                                bar->boundingRect().width() +100,
//                                bar->boundingRect().height() +100,
//                                Qt::IgnoreAspectRatio);
//    }
//  }
}

/*******************************************************************************
 * Run action.
 ******************************************************************************/
void QuickFlow::on_actionRun_triggered()
{

}

/*******************************************************************************
 * About action.
 ******************************************************************************/
void QuickFlow::on_actionAbout_triggered()
{
  About about(this);
  about.exec();
}

void QuickFlow::on_editBar(QObject *bar)
{
  BarProperties barProperties(this);
  barProperties.setOptions(project, static_cast<Bar *>(bar));

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

void QuickFlow::on_editLine(QObject *line)
{
  LineProperties lineProperties(this);
  lineProperties.setOptions(project, static_cast<Line *>(line));

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Close event.
 ******************************************************************************/
void QuickFlow::closeEvent(QCloseEvent *event)
{
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

/*******************************************************************************
 * Load application settings.
 ******************************************************************************/
void QuickFlow::loadSettings()
{
  // systemView background.
  QBrush bkbrush;
  bkbrush.setColor(settings->value("systemView_background").value<QColor>());
  bkbrush.setStyle(Qt::SolidPattern);
  ui->systemView->setBackgroundBrush(bkbrush);
}

/*******************************************************************************
 * Save application settings.
 ******************************************************************************/
void QuickFlow::saveSettings()
{
  // systemView background.
  settings->setValue("systemView_background", ui->systemView->backgroundBrush());

  // Save settings.
  settings->sync();
}

/*******************************************************************************
 * Create application settings.
 ******************************************************************************/
void QuickFlow::createSettings()
{
  // Save version.
  settings->setValue("version", kVersion);

//  systemView background.
  QBrush bkbrush;
  bkbrush.setColor(Qt::white);
  bkbrush.setStyle(Qt::SolidPattern);
  ui->systemView->setBackgroundBrush(bkbrush);

  // Store settings.
  saveSettings();
}

/*******************************************************************************
 * Upgrade application settings.
 ******************************************************************************/
void QuickFlow::upgradeSettings()
{
  // Remove everything and create a default.
  settings->clear();
  createSettings();
}

/*******************************************************************************
 * No project interface.
 ******************************************************************************/
void QuickFlow::noProjectInterface()
{
  // Project actions.
  ui->actionNew->setEnabled(true);
  ui->actionOpen->setEnabled(true);
  ui->actionSave->setEnabled(false);
  ui->actionSave_as->setEnabled(false);
  ui->actionClose->setEnabled(false);
  ui->actionSettings->setEnabled(false);

  // Simulation actions.
  ui->actionRun->setEnabled(false);
  ui->actionExport->setEnabled(false);

  // Zoom actions.
  ui->actionZoomFit->setEnabled(false);
  ui->actionZoomIn->setEnabled(false);
  ui->actionZoomOut->setEnabled(false);

  // Power network actions
  ui->actionAdd_Network->setEnabled(false);
  ui->actionAddBar->setEnabled(false);
  ui->actionAddLine->setEnabled(false);
  ui->actionSearch_Bar->setEnabled(false);

  // Algorithms actions.
  ui->actionShirmoharmmadi->setEnabled(false);
  ui->actionCespedes->setEnabled(false);
  ui->actionSugiyama->setEnabled(false);
  ui->actionSugiyama_Fast->setEnabled(false);
  ui->actionMulti_level->setEnabled(false);

  // Disable viewer.
  ui->systemView->setEnabled(false);
}

/*******************************************************************************
 * Work interface.
 ******************************************************************************/
void QuickFlow::workInterface()
{
  // Project actions.
  ui->actionNew->setEnabled(true);
  ui->actionOpen->setEnabled(true);
  ui->actionSave->setEnabled(false);
  ui->actionSave_as->setEnabled(true);
  ui->actionClose->setEnabled(true);
  ui->actionSettings->setEnabled(true);

  // Simulation actions.
  ui->actionRun->setEnabled(true);
  ui->actionExport->setEnabled(true);

  // Zoom actions.
  ui->actionZoomFit->setEnabled(true);
  ui->actionZoomIn->setEnabled(true);
  ui->actionZoomOut->setEnabled(true);

  // Power network actions
  ui->actionAdd_Network->setEnabled(true);
  ui->actionAddBar->setEnabled(true);
  ui->actionAddLine->setEnabled(true);
  ui->actionSearch_Bar->setEnabled(true);

  // Algorithms actions.
  ui->actionShirmoharmmadi->setEnabled(true);
  ui->actionCespedes->setEnabled(true);
  ui->actionSugiyama->setEnabled(true);
  ui->actionSugiyama_Fast->setEnabled(true);
  ui->actionMulti_level->setEnabled(true);

  // Enable viewer
  ui->systemView->setEnabled(true);
}

/*******************************************************************************
 * Connect Signals.
 ******************************************************************************/
void QuickFlow::connectSignals()
{
  connect(ui->systemView, SIGNAL(barProperties(QObject *)), this,
          SLOT(on_editBar(QObject *)));

  connect(ui->systemView, SIGNAL(lineProperties(QObject *)), this,
          SLOT(on_editLine(QObject *)));
}

/*******************************************************************************
 * Disconnect Signals.
 ******************************************************************************/
void QuickFlow::disconnectSignals()
{
  disconnect(ui->systemView, SIGNAL(barProperties(QObject *)), this,
             SLOT(on_editBar(QObject *)));

  disconnect(ui->systemView, SIGNAL(lineProperties(QObject *)), this,
             SLOT(on_editLine(QObject *)));
}

/*!
 * \}
 */

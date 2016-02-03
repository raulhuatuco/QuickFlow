/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 David Krepsky
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

#include <QtCore>

#include "window/projectproperties.h"
#include "window/networkproperties.h"
#include "window/barproperties.h"
#include "window/lineproperties.h"
#include "window/about.h"
#include "window/searchbar.h"

#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

#include "algorithms/import.h"
#include "algorithms/redrawnetwork.h"
#include "algorithms/shirmoharmmadi.h"
#include "algorithms/cespedes.h"
#include "algorithms/barreduction.h"

/*****************************************************************************
* Const.
*****************************************************************************/
extern QString version;

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

  // Check for settings existence and version.
  settings = new QSettings(this);

  if (settings->contains("version")) {
    if (settings->value("version").toString() == version)
      // If exists and is in the current version, use it.
      loadSettings();
    else
      // Update to the new program version.
      upgradeSettings();
  } else {
    // If there is no settings availiable, create a new one with default values.
    createSettings();
  }

  QActionGroup *solvers = new QActionGroup(this);
  solvers->setExclusive(true);
  solvers->addAction(ui->actionShirmoharmmadi);
  solvers->addAction(ui->actionCespedes);
  ui->menuSolver->addAction(ui->actionShirmoharmmadi);
  ui->menuSolver->addAction(ui->actionCespedes);



  QFile f(":theme/bluesky/bluesky.qss");

  if (!f.exists()) {
    printf("Unable to set stylesheet, file not found\n");
  } else {
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    qApp->setStyleSheet(ts.readAll());
  }

  QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
  mainLayout->setSpacing(0);
  mainLayout->setMargin(0);
  mainLayout->setContentsMargins(0,0,0,0);
  mainLayout->addWidget(ui->systemView);

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
  ui->systemView->scene()->update();
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

  ProjectProperties projectProperties(this);
  projectProperties.setOptions(NULL);

  // If canceled return
  if (projectProperties.exec() != QDialog::Accepted) {
    return;
  }

  // Save project pointer
  project = projectProperties.project();

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + project->name);
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
  projectFile.setNameFilter(tr("QuickFlow Project (*.qkf)"));
  QString initialPath = QStandardPaths::standardLocations(
                          QStandardPaths::DocumentsLocation)[0] + "/QuickFlow";

  projectFile.setDirectory(initialPath);

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
  setWindowTitle("QuickFlow - " + project->name);
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
  projectFile.setNameFilter(tr("QuickFlow Project (*.qkf)"));
  QString initialPath = QStandardPaths::standardLocations(
                          QStandardPaths::DocumentsLocation)[0] + "/QuickFlow";
  projectFile.setDirectory(initialPath);

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

  delete project;
  project = NULL;
  altered_ = false;
  noProjectInterface();
  setWindowTitle("QuickFlow");

  ui->systemView->zoomFit();
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

  if(barProperties.setOptions(project, NULL))
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

  if(lineProperties.setOptions(project, NULL))
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

  // Try to import network.
  Network *network = new Network;

  bool importOk = importTxtType1(fileName, network);

  // Impor success.
  if(importOk) {
    project->networks.insert(network->name, network);

    NetworkProperties newNetwork(this);
    newNetwork.setOptions(project, network);

    if (newNetwork.exec() == QDialog::Rejected) {
      project->networks.remove(network->name);
      delete network;
      return;
    }

    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    delete network;
    return;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();
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

  // Try to import network.
  Network *network = new Network;

  bool importOk = importTxtType2(fileName, network);

  // Impor success.
  if(importOk) {
    project->networks.insert(network->name, network);

    NetworkProperties newNetwork(this);
    newNetwork.setOptions(project, network);

    if (newNetwork.exec() == QDialog::Rejected) {
      project->networks.remove(network->name);
      delete network;
      return;
    }

    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    delete network;
    return;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();
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

  // Try to import network.
  Network *network = new Network;

  bool importOk = importTxtType3(fileName, network);

  // Impor success.
  if(importOk) {
    project->networks.insert(network->name, network);

    NetworkProperties newNetwork(this);
    newNetwork.setOptions(project, network);

    if (newNetwork.exec() == QDialog::Rejected) {
      project->networks.remove(network->name);
      delete network;
      return;
    }

    ui->systemView->addNetwork(network);
    ui->systemView->zoomFit();
    setAltered(true);
  } else {
    delete network;
    return;
  }

  RedrawNetwork redraw(network);
  redraw.sugiyamaFast();
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
    RedrawNetwork redraw(network);
    redraw.sugiyama();
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
    RedrawNetwork redraw(network);
    redraw.sugiyamaFast();
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
    RedrawNetwork redraw(network);
    redraw.multiLevel();
  }

  setAltered(true);
  ui->systemView->zoomFit();
}

/*******************************************************************************
 * Search bar action.
 ******************************************************************************/
void QuickFlow::on_actionSearch_Bar_triggered()
{
  SearchBar searchBar(this);
  searchBar.setOptions(project);

  if (searchBar.exec() == QDialog::Accepted) {
    QRectF fitRect = searchBar.bar()->sceneBoundingRect();

    fitRect.setX(fitRect.x() -100);
    fitRect.setY(fitRect.y() - 100);
    fitRect.setWidth(fitRect.width() + 100);
    fitRect.setHeight(fitRect.height() + 100);

    ui->systemView->fitInView(fitRect, Qt::KeepAspectRatio);
  }
}

/*******************************************************************************
 * Run action.
 ******************************************************************************/
void QuickFlow::on_actionRun_triggered()
{
  foreach(Network *network, project->networks) {
    BarReduction optimizer;

    if(ui->actionOptimize_Network->isChecked()) {
      optimizer.setNetwork(network);
      int32_t rbars = optimizer.reduce();
      QMessageBox::information(this, "Bar reduction",
                               "Reduced: " + QString::number(rbars) + " bars \n",
                               QMessageBox::Ok);
    }

    if(ui->actionShirmoharmmadi->isChecked()) {

      Shirmoharmmadi fluxo(network);
      fluxo.solve();

      QMessageBox::information(this, "Duration",
                               "Simulation duration: " +
                               QString::number(fluxo.duration) + " [ms] \n" +
                               "Used iterations: " + QString::number(fluxo.usedIterations),
                               QMessageBox::Ok);
    } else {
      Cespedes fluxo(network);
      fluxo.solve();
      QMessageBox::information(this, "Duration",
                               "Simulation duration: " +
                               QString::number(fluxo.duration) + " [ms] \n" +
                               "Used iterations: " + QString::number(fluxo.usedIterations)
                               + "\nTotal Loss: " + QString::number(fluxo.totalLoss.real()
                                   *network->powerBase()) + " Watts, " +  QString::number(fluxo.totalLoss.imag()
                                       *network->powerBase()) + " VAR, ",
                               QMessageBox::Ok);
    }

    if(ui->actionOptimize_Network->isChecked()) {
      optimizer.expand();
    }
  }

  setAltered(true);
}

/*******************************************************************************
 * Settings action.
 ******************************************************************************/
void QuickFlow::on_actionSettings_triggered()
{
  ProjectProperties projectProperties(this);

  projectProperties.setOptions(project);

  projectProperties.exec();
}

/*******************************************************************************
 * Export image action.
 ******************************************************************************/
void QuickFlow::on_actionExport_image_triggered()
{

  // Get file location from user.
  QFileDialog imgFile(this);
  imgFile.setAcceptMode(QFileDialog::AcceptSave);
  QStringList filters;
  filters << "PNG (*.png)";
  filters << "JPG (*.jpg)";
  filters << "BMP (*.bmp)";

  imgFile.setNameFilters(filters);
  QString initialPath = QStandardPaths::standardLocations(
                          QStandardPaths::DocumentsLocation)[0] + "/QuickFlow";
  imgFile.setDirectory(initialPath);

  // Check if user has canceled the opening.
  if (imgFile.exec() != QDialog::Accepted) {
    return;
  }

  QString imgPath =  imgFile.selectedFiles()[0];

  double w = ui->systemView->scene()->sceneRect().width();
  double h = ui->systemView->scene()->sceneRect().height();

  QImage img(static_cast<int> (w), static_cast<int> (h),
             QImage::Format_ARGB32);
             img.fill(Qt::transparent);
  QPainter p(&img);
  p.setRenderHint(QPainter::Antialiasing);
  ui->systemView->scene()->render(&p);
  p.end();
  img.save(imgPath);
}


/*******************************************************************************
 * About action.
 ******************************************************************************/
void QuickFlow::on_actionAbout_triggered()
{
  About about(this);
  about.exec();
}

/*******************************************************************************
 * Edit bar action.
 ******************************************************************************/
void QuickFlow::on_editBar(QObject *bar)
{
  BarProperties barProperties(this);
  barProperties.setOptions(project, static_cast<Bar *>(bar));

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Edit line action.
 ******************************************************************************/
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
  settings->setValue("version", version);

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

void QuickFlow::on_actionExport_triggered()
{

}

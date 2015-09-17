#include "quickflow.h"
#include "ui_quickflow.h"

#include "pn/bar.h"
#include "pn/line.h"
#include "pn/network.h"

#include "window/newproject.h"
#include "window/barproperties.h"
#include "window/lineproperties.h"

#include "algorithms/import.h"
#include "algorithms/redraw.h"

QString const kVersion = "0.0.3";

/*******************************************************************************
 * QKflow.
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
 * ~QKflow.
 ******************************************************************************/
QuickFlow::~QuickFlow()
{
  delete ui;
}

/*******************************************************************************
 * isAltered.
 ******************************************************************************/
bool QuickFlow::isAltered()
{
  return altered_;
}

/*******************************************************************************
 * setAltered.
 ******************************************************************************/
void QuickFlow::setAltered(bool altered)
{
  altered_ = altered;

  if(altered_)
    ui->actionSave->setEnabled(true);
  else
    ui->actionSave->setEnabled(false);
}

/*******************************************************************************
 * noProjectInterface.
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
  ui->pnView->setEnabled(false);
}

/*******************************************************************************
 * workInterface.
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
  ui->pnView->setEnabled(true);
}

/*******************************************************************************
 * loadSettings.
 ******************************************************************************/
void QuickFlow::loadSettings()
{
  // pnView background.
  QBrush bkbrush;
  bkbrush.setColor(settings->value("pnView_background").value<QColor>());
  bkbrush.setStyle(Qt::SolidPattern);
  ui->pnView->setBackgroundBrush(bkbrush);
}

/*******************************************************************************
 * saveSettings.
 ******************************************************************************/
void QuickFlow::saveSettings()
{
  // pnView background.
  settings->setValue("pnView_background", ui->pnView->backgroundBrush());

  // Save settings.
  settings->sync();
}

/*******************************************************************************
 * createSettings.
 ******************************************************************************/
void QuickFlow::createSettings()
{
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
 * upgradeSettings.
 ******************************************************************************/
void QuickFlow::upgradeSettings()
{
  // Remove everything and create a default.
  settings->clear();
  createSettings();
}

/*******************************************************************************
 * Connect Signals.
 ******************************************************************************/
void QuickFlow::connectSignals()
{
  connect(project->network, SIGNAL(barProperties(QObject *)), this,
          SLOT(on_editBar(QObject *)));

  connect(project->network, SIGNAL(lineProperties(QObject *)), this,
          SLOT(on_editLine(QObject *)));
}

/*******************************************************************************
 * Disconnect Signals.
 ******************************************************************************/
void QuickFlow::disconnectSignals()
{
  disconnect(project->network, SIGNAL(barProperties(QObject *)), this,
             SLOT(on_editBar(QObject *)));

  disconnect(project->network, SIGNAL(lineProperties(QObject *)), this,
             SLOT(on_editLine(QObject *)));
}

/*******************************************************************************
 * Action New Project triggered.
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
                      newProject.dataName + ".qkflow";

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

  // Try to save project.
  bool save_ok =
    project->save();

  if (!save_ok) {
    QMessageBox::critical(this, "File write error",
                          "Cannot write .qkflow file.", QMessageBox::Ok);
    delete project;
    project = NULL;
    return;
  }

  // New project is Ok
  // Change window title to indicate that the project is open
  setWindowTitle("QkFlow - " + newProject.dataName);
  // Enable interface
  workInterface();
  // Set screne
  ui->pnView->setPnNetwork(project->network);

  // Connect signals.
  connectSignals();
}

/*******************************************************************************
 * Action Open triggered.
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
  ui->pnView->setPnNetwork(project->network);

  // Connect signals.
  connectSignals();
}

/*******************************************************************************
 * Action Save triggered.
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

  // Project doesnt have any new modifications now.
  // Disable save button.
  ui->actionSave->setEnabled(false);
  altered_ = false;
}

/*******************************************************************************
 * Action Save As triggered.
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

  // Project doesnt have any new modifications now.
  // Disable save button.
  ui->actionSave->setEnabled(false);
  altered_ = false;
}

/*******************************************************************************
* Action Close triggered.
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

  // Disconnect signals.
  disconnectSignals();

  delete project;
  project = NULL;
  altered_ = false;
  noProjectInterface();
}

/*******************************************************************************
 * Action ZoomIn triggered.
 ******************************************************************************/
void QuickFlow::on_actionZoomIn_triggered()
{
  ui->pnView->zoomIn();
}

/*******************************************************************************
 * Action ZoomOut triggered.
 ******************************************************************************/
void QuickFlow::on_actionZoomOut_triggered()
{
  ui->pnView->zoomOut();
}

/*******************************************************************************
 * Action ZoomFit triggered.
 ******************************************************************************/
void QuickFlow::on_actionZoomFit_triggered()
{
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Search Bar triggered.
 ******************************************************************************/
void QuickFlow::on_actionSearch_Bar_triggered()
{
  bool ok;
  int barId = QInputDialog::getInt(this, tr("Search for bar"), tr("Bad Id"), 0, 0,
                                   1000000, 1, &ok);

  if(ok) {
    Bar *bar = project->network->getBarById(barId);

    if (bar != NULL) {
      ui->pnView->fitInView(bar->x() -100, bar->y() -100,
                            bar->boundingRect().width() +100,
                            bar->boundingRect().height() +100,
                            Qt::IgnoreAspectRatio);
    }
  }
}

/*******************************************************************************
 * Action Add Bar triggered.
 ******************************************************************************/
void QuickFlow::on_actionAddBar_triggered()
{
  BarProperties barProperties(this);
  barProperties.setOptions(project, NULL);

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Action Add Line triggered.
 ******************************************************************************/
void QuickFlow::on_actionAddLine_triggered()
{
  LineProperties lineProperties(this);
  lineProperties.setOptions(project, NULL);

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Action Import Txt Type 1 triggered.
 ******************************************************************************/
void QuickFlow::on_action_txt_type_1_triggered()
{
// Check if there is a project already opened.
  if (project != NULL) {
    // Close project.
    ui->actionClose->trigger();

    // If close didnt succed, cancel import.
    if (project != NULL) return;
  }

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

  // Try to import.
  project = importTxtType1(fileName);

  if(project == NULL) {
    noProjectInterface();
  } else {
    workInterface();
    setAltered(true);
    ui->pnView->setPnNetwork(project->network);
    connectSignals();
    ui->pnView->zoomFit();
  }
}

/*******************************************************************************
 * Action Import Txt Type 2 triggered.
 ******************************************************************************/
void QuickFlow::on_action_txt_type_2_triggered()
{
  // Check if there is a project already opened.
  if (project != NULL) {
    // Close project.
    ui->actionClose->trigger();

    // If close didnt succed, cancel import.
    if (project != NULL) return;
  }

  // Get file location from user.
  QFileDialog txtFile(this);
  txtFile.setAcceptMode(QFileDialog::AcceptOpen);
  txtFile.setFileMode(QFileDialog::ExistingFile);
  txtFile.setNameFilter(tr("Text File Type 2(*.txt)"));
  txtFile.setDirectory(QStandardPaths::standardLocations(
                         QStandardPaths::HomeLocation)[0]);

  // Check if user has canceled the opening.
  if (txtFile.exec() != QDialog::Accepted) {
    return;
  }

  // Grab filename.
  QString fileName = txtFile.selectedFiles()[0];

  // Try to import.
  project = importTxtType2(fileName);

  if(project == NULL) {
    noProjectInterface();
  } else {
    workInterface();
    setAltered(true);
    ui->pnView->setPnNetwork(project->network);
    connectSignals();
    ui->pnView->zoomFit();
  }
}

/*******************************************************************************
 * Action Cespedes triggered.
 ******************************************************************************/
void QuickFlow::on_actionCespedes_triggered()
{
  if(ui->actionShirmoharmnadi->isChecked()) {
    ui->actionShirmoharmnadi->setChecked(false);
  }
}

/*******************************************************************************
 * Action Shirmoharmnadi triggered.
 ******************************************************************************/
void QuickFlow::on_actionShirmoharmnadi_triggered()
{
  if(ui->actionCespedes->isChecked()) {
    ui->actionCespedes->setChecked(false);
  }
}

/*******************************************************************************
 * Action Pause triggered.
 ******************************************************************************/
void QuickFlow::on_actionPause_triggered()
{
  uint32_t cnt = 0;
  Bar *bar = new Bar;
  bar->id = 0;
  project->network->addBar(bar);
  cnt++;

  for(int x=0; x< 100; x++) {
    for (int y = 0; y< 100; y++) {
      bar = new Bar;
      bar->id = cnt;
      bar->setX(x*50);
      bar->setY(0);
      project->network->addBar(bar);
      PnLine *line = new PnLine;
      line->noI = cnt -1;
      line->noF = cnt;
      project->network->addLine(line);
      cnt++;
    }
  }
}

/*******************************************************************************
 * Action Redraw Sigiyama.
 ******************************************************************************/
void QuickFlow::on_actionSugiyama_triggered()
{
  redrawGraph2(project->network);
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Redraw Sigiyama Fast.
 ******************************************************************************/
void QuickFlow::on_actionSugiyama_Fast_triggered()
{
  redrawGraph1(project->network);
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Redraw MultiLevel.
 ******************************************************************************/
void QuickFlow::on_actionMulti_level_triggered()
{
  redrawGraph3(project->network);
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Exit triggered.
 ******************************************************************************/
void QuickFlow::on_actionExit_triggered()
{
  close();
}

/*******************************************************************************
 * closeEvent.
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
 * Bar Double Click.
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
 * Line Double Click.
 ******************************************************************************/
void QuickFlow::on_editLine(QObject *line)
{
  LineProperties lineProperties(this);
  lineProperties.setOptions(project, static_cast<PnLine *>(line));

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}


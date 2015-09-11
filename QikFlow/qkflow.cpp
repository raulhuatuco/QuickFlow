#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextStream>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnline.h"
#include "PnGraphics/pnnetwork.h"

#include "window/newproject.h"
#include "window/barproperties.h"
#include "window/lineproperties.h"

QString const kVersion = "0.0.3";

/*******************************************************************************
 * QKflow.
 ******************************************************************************/
QKflow::QKflow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::QKflow),
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
QKflow::~QKflow()
{
  delete ui;
}

/*******************************************************************************
 * isAltered.
 ******************************************************************************/
bool QKflow::isAltered()
{
  return altered_;
}

/*******************************************************************************
 * setAltered.
 ******************************************************************************/
void QKflow::setAltered(bool altered)
{
  altered_ = altered;

  if(altered_)
    ui->actionSave->setEnabled(true);
}

/*******************************************************************************
 * noProjectInterface.
 ******************************************************************************/
void QKflow::noProjectInterface()
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
void QKflow::workInterface()
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
void QKflow::loadSettings()
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
void QKflow::saveSettings()
{
  // pnView background.
  settings->setValue("pnView_background", ui->pnView->backgroundBrush());

  // Save settings.
  settings->sync();
}

/*******************************************************************************
 * createSettings.
 ******************************************************************************/
void QKflow::createSettings()
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
void QKflow::upgradeSettings()
{
  // Remove everything and create a default.
  settings->clear();
  createSettings();
}

/*******************************************************************************
 * Connect Project.
 ******************************************************************************/
void QKflow::connectProject()
{
  connect(project->pnNetwork, SIGNAL(barProperties(QObject *)), this,
          SLOT(on_editBar(QObject *)));

  connect(project->pnNetwork, SIGNAL(lineProperties(QObject *)), this,
          SLOT(on_editLine(QObject *)));
}

/*******************************************************************************
 * Disconnect Project.
 ******************************************************************************/
void QKflow::disconnectProject()
{
  disconnect(project->pnNetwork, SIGNAL(barProperties(QObject *)), this,
             SLOT(on_editBar(QObject *)));

  disconnect(project->pnNetwork, SIGNAL(lineProperties(QObject *)), this,
             SLOT(on_editLine(QObject *)));
}

/*******************************************************************************
 * Action New Project triggered.
 ******************************************************************************/
void QKflow::on_actionNew_triggered()
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
  project->filepath = newProject.dataPath + QDir::separator() +
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
  ui->pnView->setPnNetwork(project->pnNetwork);

  // Connect signals.
  connectProject();
}

/*******************************************************************************
 * Action Open triggered.
 ******************************************************************************/
void QKflow::on_actionOpen_triggered()
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

  // Connect signals.
  connectProject();
}

/*******************************************************************************
 * Action Save triggered.
 ******************************************************************************/
void QKflow::on_actionSave_triggered()
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
void QKflow::on_actionSave_as_triggered()
{
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
void QKflow::on_actionClose_triggered()
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
  disconnectProject();

  delete project;
  project = NULL;
  altered_ = false;
  noProjectInterface();
}

/*******************************************************************************
 * Action ZoomIn triggered.
 ******************************************************************************/
void QKflow::on_actionZoomIn_triggered()
{
  ui->pnView->zoomIn();
}

/*******************************************************************************
 * Action ZoomOut triggered.
 ******************************************************************************/
void QKflow::on_actionZoomOut_triggered()
{
  ui->pnView->zoomOut();
}

/*******************************************************************************
 * Action ZoomFit triggered.
 ******************************************************************************/
void QKflow::on_actionZoomFit_triggered()
{
  ui->pnView->zoomFit();
}

/*******************************************************************************
 * Action Add Bar triggered.
 ******************************************************************************/
void QKflow::on_actionAddBar_triggered()
{
  // Create a new bar that will be used by bar properties window.
  PnBar *bar = new PnBar;

  BarProperties barProperties(this);
  barProperties.setBar(bar, true);
  barProperties.setUnit(project->pnNetwork->voltageUnit,
                        project->pnNetwork->powerUnit);

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);

    if(!project->pnNetwork->addBar(bar)) {
      QMessageBox::critical(this, "Invalid Bar", "Bar id already in use.",
                            QMessageBox::Ok);
      delete bar;
    }
  } else {
    delete bar;
  }
}

/*******************************************************************************
 * Action Add Line triggered.
 ******************************************************************************/
void QKflow::on_actionAddLine_triggered()
{
  PnLine *line = new PnLine;

  LineProperties lineProperties(this);
  lineProperties.setLine(line, true);
  lineProperties.setBarMap(project->pnNetwork->barMap);
  lineProperties.setUnit(project->lengthUn(), project->impedanceUn());

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);

    if(!project->pnNetwork->addLine(line)) {
      QMessageBox::critical(this, "Invalid line",
                            "A line is already connected to this nodes.",
                            QMessageBox::Ok);
      delete line;
    }
  } else {
    delete line;
  }
}

/*******************************************************************************
 * Action Exit triggered.
 ******************************************************************************/
void QKflow::on_actionExit_triggered()
{
  close();
}

/*******************************************************************************
 * closeEvent.
 ******************************************************************************/
void QKflow::closeEvent(QCloseEvent *event)
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
void QKflow::on_editBar(QObject *bar)
{
  BarProperties barProperties(this);
  barProperties.setBar(static_cast<PnBar *>(bar), false);
  barProperties.setUnit(project->voltageUn(), project->powerUn());

  if (barProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

/*******************************************************************************
 * Line Double Click.
 ******************************************************************************/
void QKflow::on_editLine(QObject *line)
{
  LineProperties lineProperties(this);
  lineProperties.setLine(static_cast<PnLine *>(line), false);
  lineProperties.setUnit(project->lengthUn(), project->impedanceUn());

  if (lineProperties.exec() == QDialog::Accepted) {
    setAltered(true);
  }
}

void QKflow::on_action_txt_type_1_triggered()
{
// Check if there is a project already opened.
  if (project != NULL) {
    // Close project.
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
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


  // Create Project.
  project = new Project;

  // Try to import.
  if(!import1(fileName)) {
    delete project;
    project = NULL;
  } else {
    workInterface();
    setAltered(true);
  }

}

void QKflow::on_action_txt_type_2_triggered()
{
// Check if there is a project already opened.
  if (project != NULL) {
    // Close project.
    ui->actionClose->trigger();

    // If close didnt succed, cancel new project.
    if (project != NULL) return;
  }

//------------------------------------------------------------------------------
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
  import2(fileName);
}

bool QKflow::import1(QString &fileName)
{
// Open File.
//------------------------------------------------------------------------------
  QFile qfile(fileName);

  if(!qfile.open(QFile::ReadOnly)) {
    QMessageBox::critical(this, "File read error.",
                          "Can\'t read " + fileName + ".",
                          QMessageBox::Ok);
    return false;
  }

// Get project name.
//------------------------------------------------------------------------------
  int32_t nameInit = fileName.lastIndexOf(QDir::separator());
  int32_t nameFinal = fileName.lastIndexOf('.');
  project->name = fileName.mid(nameInit+1, nameFinal- nameInit -1);

// Get project path.
//------------------------------------------------------------------------------
  project->filepath = fileName;

// Auxiliaty variables.
//------------------------------------------------------------------------------
  QTextStream stream(&qfile); // Text stream used to read lines.
  int32_t lineCnt = 0; // Current line counter, used to report errors.
  QString lineTxt; // Line text.
  QString dummy; // Use to dump useless data.

// Read Number of bars.
//------------------------------------------------------------------------------
  // Loop until NumBarras is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("NumBarras"));

  // Get Number of bars.
  uint32_t numBar;
  stream >> numBar;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Read Number of lines.
//------------------------------------------------------------------------------
  // Loop until NumLinhas is found or EoF.
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("NumLinhas"));

  // Get Number of lines.
  uint32_t numLine;
  stream >> numLine;
  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Base Voltage.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("Voltagem_base_da_rede"));

  // Get VBase.
  double VBase;
  stream >> VBase;
  // Save to project.
  project->setVoltageBase(VBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Length Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("comprimento_das_linhas"));

  // Get Length Unit.
  uint32_t LenUn;
  stream >> LenUn;

  // Cast to Unit enum.
  switch (LenUn) {
  case 1:
    project->setLengthUn(Unit::kMeter);
    break;

  case 2:
    project->setLengthUn(Unit::kKiloMeter);
    break;

  case 3:
    project->setLengthUn(Unit::kFeet);
    break;

  case 4:
    project->setLengthUn(Unit::kMile);
    break;

  default:
    project->setLengthUn(Unit::kMeter);
    break;
  }

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Impedance Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("impedancia"));

  // Get Impedance Unit.
  uint32_t ImpUn;
  stream >> ImpUn;

  // Save to project.
  switch (ImpUn) {
  case 1:
    project->setImpedanceUn(Unit::kOhm);
    break;

  case 2:
    project->setImpedanceUn(Unit::kOhmPerMeter);
    break;

  case 3:
    project->setImpedanceUn(Unit::kOhmPerKilometer);
    break;

  case 4:
    project->setImpedanceUn(Unit::kOhmPerFeet);
    break;

  case 5:
    project->setImpedanceUn(Unit::kOhmPerMile);
    break;

  default:
    project->setImpedanceUn(Unit::kOhm);
    break;
  }

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get power Unit.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("Demanda"));

  // Get power Unit.
  uint32_t powerUn;
  stream >> powerUn;

  // Save to project.
  switch (powerUn) {
  case 1:
    project->setPowerUn(Unit::kVA);
    break;

  case 2:
    project->setPowerUn(Unit::kKiloVA);
    break;

  case 3:
    project->setPowerUn(Unit::kMegaVa);
    break;

  default:
    project->setPowerUn(Unit::kKiloVA);
    break;
  }

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Voltage Unit.
//------------------------------------------------------------------------------
  project->setVoltageUn(Unit::kKiloVolts);

// Get Base Power.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("kVA    kV_hihg "));

  // Get PBase.
  double PBase;
  stream >> dummy >> PBase;
  // Save.
  project->setPowerBase(PBase);

  // Go to the next line.
  stream.readLine();
  lineCnt++;

// Get Bars.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("[n"));

  uint32_t id;
  double Sla, Slai;
  double Slb, Slbi;
  double Slc, Slci;
  double Sga, Sgai;
  double Sgb, Sgbi;
  double Sgc, Sgci;

  PnBar *bar;

  for(uint32_t i=0; i<numBar; i++) {
    lineTxt = stream.readLine();
    lineCnt++;

    stream >> id >> dummy >> Sla >> Slai >> Slb >> Slbi >> Slc >> Slci;
    stream >> Sga >> Sgai >> Sgb >> Sgbi >> Sgc >>Sgci;
    bar = new PnBar;
    bar->id = id - 1;
    bar->Sga.real(Sga);
    bar->Sga.imag(Sgai);
    bar->Sgb.real(Sgb);
    bar->Sgb.imag(Sgbi);
    bar->Sgc.real(Sgc);
    bar->Sgc.imag(Sgci);
    bar->Sla.real(Sla);
    bar->Sla.imag(Slai);
    bar->Slb.real(Slb);
    bar->Slb.imag(Slbi);
    bar->Slc.real(Slc);
    bar->Slc.imag(Slci);

    if (!project->pnNetwork->addBar(bar)) {
      QMessageBox::critical(this, "Invalid Bar.",
                            "Invalid Bar " + QString::number(id) + "at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete bar;
      return false;
    }
  }

// Get Lines.
//------------------------------------------------------------------------------
  do {
    lineTxt = stream.readLine();
    lineCnt++;

    if(stream.atEnd()) {
      QMessageBox::critical(this, "Invalid File.",
                            "The file " + fileName + " is invalid.",
                            QMessageBox::Ok);
      return false;
    }
  } while(!lineTxt.contains("Tipo_Cir"));

  uint32_t noI, noF;
  double Zaa, Zaai;
  double Zab, Zabi;
  double Zac, Zaci;
  double Zan, Zani;
  double Zbb, Zbbi;
  double Zbc, Zbci;
  double Zbn, Zbni;
  double Zcc, Zcci;
  double Zcn, Zcni;
  double Znn, Znni;
  double length;

  PnLine *line;

  for(uint32_t i=0; i<numBar; i++) {
    lineTxt = stream.readLine();
    lineCnt++;

    stream >> dummy >> noI >> noF >> Zaa >> Zaai >> Zab >> Zabi >> Zac;
    stream >> Zaci >> Zan >> Zani >> Zbb >> Zbbi >> Zbc >> Zbci >> Zbn >> Zbni;
    stream >> Zcc >> Zcci >> Zcn >> Zcni >> Znn >> Znni >> dummy >> length;

    line = new PnLine;
    line->setNodes(project->pnNetwork->getBarById(noI - 1),
                   project->pnNetwork->getBarById(noF - 1));

    line->length = length;
    line->Zaa.real(Zaa);
    line->Zaa.imag(Zaai);
    line->Zab.real(Zab);
    line->Zab.imag(Zabi);
    line->Zac.real(Zac);
    line->Zac.imag(Zaci);
    line->Zan.real(Zan);
    line->Zan.imag(Zani);
    line->Zbb.real(Zbb);
    line->Zbb.imag(Zbbi);
    line->Zbc.real(Zbc);
    line->Zbc.imag(Zbci);
    line->Zbn.real(Zbn);
    line->Zbn.imag(Zbni);
    line->Zcc.real(Zcc);
    line->Zcc.imag(Zcci);
    line->Zcn.real(Zcn);
    line->Zcn.imag(Zcni);
    line->Znn.real(Znn);
    line->Znn.imag(Znni);

    if (!project->pnNetwork->addLine(line)) {
      QMessageBox::critical(this, "Invalid Line.",
                            "Invalid Line at line " +
                            QString::number(lineCnt) + ".",
                            QMessageBox::Ok);
      delete line;
      return false;
    }
  }

  project->pnNetwork->redrawAlgorithm1();

  qfile.close();
  return true;
}

bool QKflow::import2(QString &fileName)
{
  Q_UNUSED(fileName);
  return false;
}

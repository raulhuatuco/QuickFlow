#include "newproject.h"
#include "ui_newproject.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QValidator>

/*******************************************************************************
 * WindowNewProject
 ******************************************************************************/
NewProject::NewProject(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::NewProject) {
  // Set-up user interface.
  ui->setupUi(this);

  // Create validator for data input.
  ui->maxIterations->setValidator(new QIntValidator(1,1000000, this));
  ui->minError->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->voltageBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->powerBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));

  // Populate comboboxes.
  ui->lengthUnit->addItem("m", 1.0);
  ui->lengthUnit->addItem("km", 1.0e3);
  ui->lengthUnit->addItem("in", 39.3701);
  ui->lengthUnit->addItem("ft", 3.28084);
  ui->lengthUnit->addItem("mile", 0.000621371);

  ui->impedanceUnit->addItem("Ohm");
  ui->impedanceUnit->addItem("Ohm/m");
  ui->impedanceUnit->addItem("Ohm/km");
  ui->impedanceUnit->addItem("Ohm/in");
  ui->impedanceUnit->addItem("Ohm/ft");
  ui->impedanceUnit->addItem("Ohm/mile");

  ui->voltageUnit->addItem("V", 1.0);
  ui->voltageUnit->addItem("kV", 1.0e3);
  ui->voltageUnit->addItem("MV", 1.0e6);

  ui->powerUnit->addItem("VA", 1.0);
  ui->powerUnit->addItem("kVA", 1.0e3);
  ui->powerUnit->addItem("MVA", 1.0e6);
  ui->powerUnit->addItem("GVA", 1.0e9);
}

/*******************************************************************************
 * ~WindowNewProject
 ******************************************************************************/
NewProject::~NewProject() {
  delete ui;
}

/*******************************************************************************
 * Text Changed.
 ******************************************************************************/
void NewProject::on_name_textChanged(const QString &arg1) {
  // Display new name.
  ui->outputFile->setText(ui->path->text() + QDir::separator() + arg1 +
                          ".qkflow");
}

/*******************************************************************************
 * Path Changed.
 ******************************************************************************/
void NewProject::on_path_textChanged(const QString &arg1) {
  // Display new path.
  ui->outputFile->setText(arg1 + QDir::separator() + ui->name->text() +
                          ".qkflow");
}

/*******************************************************************************
 * Search Path Button clicked.
 ******************************************************************************/
void NewProject::on_toolButton_clicked() {
  // Set file dialog options.
  QFileDialog directory(this);
  directory.setFileMode(QFileDialog::Directory);
  directory.setOption(QFileDialog::ShowDirsOnly);
  directory.setAcceptMode(QFileDialog::AcceptOpen);
  directory.setDirectory(QStandardPaths::standardLocations(
                           QStandardPaths::HomeLocation)[0]);
  directory.setWindowTitle("Project path");


  // Execute. If path is selected, change ui->path to set the new directory.
  if (directory.exec())
    ui->path->setText(directory.selectedFiles()[0]);
}

/*******************************************************************************
 * Button Box accepted.
 ******************************************************************************/
void NewProject::on_buttonBox_accepted() {
  // Check for empty name.
  if (ui->name->text().isEmpty()) {
    QMessageBox::information(this, "Invalid name", "Invalid project name",
                             QMessageBox::Ok);
    ui->name->setFocus();
    return;
  }

  // Check for empty path.
  if (ui->path->text().isEmpty()) {
    QMessageBox::information(this, "Invalid path", "Invalid project path",
                             QMessageBox::Ok);
    ui->path->setFocus();
    return;
  }

  // Check for empty max iterations.
  if(ui->maxIterations->text().isEmpty()) {
    QMessageBox::information(this, "Invalid max iterations",
                             "Select a maximum number of iterations.",
                             QMessageBox::Ok);
    ui->maxIterations->setFocus();
    return;
  }

  // Check for empty min error.
  if(ui->minError->text().isEmpty()) {
    QMessageBox::information(this, "Invalid min error",
                             "Select a minimum error.",
                             QMessageBox::Ok);
    ui->minError->setFocus();
    return;
  }

  // Check for empty voltage base.
  if(ui->voltageBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid voltage base",
                             "Select a voltage base.",
                             QMessageBox::Ok);
    ui->voltageBase->setFocus();
    return;
  }

  // Check for empty power base.
  if(ui->powerBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid power base",
                             "Select a power base.",
                             QMessageBox::Ok);
    ui->powerBase->setFocus();
    return;
  }

// Return to parent window.
  accept();
}

/*******************************************************************************
 * Button Box rejected.
 ******************************************************************************/
void NewProject::on_buttonBox_rejected() {
// Return to parent window.
  reject();
}

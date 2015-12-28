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
 * \file projectproperties.cpp
 *
 * \brief Implementation of the ProjectProperties class.
 *
 * This file contains the implementation for the ProjectProperties class.
 *
 * \author David Krepsky
 * \version 0.3
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "projectproperties.h"
#include "ui_projectproperties.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QValidator>
#include "unit.h"
#include "math_constants.h"

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
ProjectProperties::ProjectProperties(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::ProjectProperties),
    isNew(false)
{
  // Set-up user interface.
  ui->setupUi(this);

  // Create validator for data input.
  ui->maxIterations->setValidator(new QIntValidator(1,1000000, this));
  ui->minError->setValidator(new QDoubleValidator(0, qInf(), 1000, this));

  // Populate comboboxes.
  ui->lengthUnit->addItem("m", qVariantFromValue(Unit::kMeter));
  ui->lengthUnit->addItem("km", qVariantFromValue(Unit::kKiloMeter));
  ui->lengthUnit->addItem("ft", qVariantFromValue(Unit::kFeet));
  ui->lengthUnit->addItem("mile", qVariantFromValue(Unit::kMile));

  ui->impedanceUnit->addItem("Ohm", qVariantFromValue(Unit::kOhm));
  ui->impedanceUnit->addItem("Ohm/m", qVariantFromValue(Unit::kOhmPerMeter));
  ui->impedanceUnit->addItem("Ohm/km", qVariantFromValue(Unit::kOhmPerKilometer));
  ui->impedanceUnit->addItem("Ohm/ft", qVariantFromValue(Unit::kOhmPerFeet));
  ui->impedanceUnit->addItem("Ohm/mile", qVariantFromValue(Unit::kOhmPerMile));

  ui->voltageUnit->addItem("V", qVariantFromValue(Unit::kVolts));
  ui->voltageUnit->addItem("kV", qVariantFromValue(Unit::kKiloVolts));

  ui->powerUnit->addItem("VA", qVariantFromValue(Unit::kVA));
  ui->powerUnit->addItem("kVA", qVariantFromValue(Unit::kKiloVA));
  ui->powerUnit->addItem("MVA", qVariantFromValue(Unit::kMegaVa));

  ui->currentUnit->addItem("A", qVariantFromValue(Unit::kAmpere));
  ui->currentUnit->addItem("kA", qVariantFromValue(Unit::kKiloAmpere));

  // Set initial path.
  QString initialPath = QStandardPaths::standardLocations(
                          QStandardPaths::DocumentsLocation)[0] + "/QuickFlow";
  ui->path->setText(initialPath);
  
  // Set search path button style.
  ui->location->setProperty("searchFile", true);
}

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
ProjectProperties::~ProjectProperties()
{
  delete ui;
}

/*******************************************************************************
 * Set options.
 ******************************************************************************/
void ProjectProperties::setOptions(Project *project)
{
  // Check if it's to create a new project or to edit one and changes the
  // interface accordingly.
  if(project == NULL) {
    // Create a new project
    project_ = new Project;
    setWindowTitle("New Project");
    isNew = true;

  } else {
    // Edit a project.
    project_ = project;
    setWindowTitle("Edit " + project->name);

    // Load project data to fill the interface.
    ui->name->setText(project->name);

    QString projectPath = project->filePath.left(project->filePath.lastIndexOf(
                            QDir::separator()));
    ui->path->setText(projectPath);

    ui->maxIterations->setText(QString::number(project->maxIterations()));
    ui->minError->setText(QString::number(project->minError()*100.0));

    ui->voltageUnit->setCurrentIndex(project->voltageUnit());
    ui->powerUnit->setCurrentIndex(project->powerUnit());
    ui->lengthUnit->setCurrentIndex(project->lengthUnit());
    ui->currentUnit->setCurrentIndex(project->currentUnit());
    ui->impedanceUnit->setCurrentIndex(project->impedanceUnit());

    isNew = false;
  }
}

/*******************************************************************************
 * Project.
 ******************************************************************************/
Project *ProjectProperties::project()
{
  return project_;
}

/*******************************************************************************
 * Text Changed.
 ******************************************************************************/
void ProjectProperties::on_name_textChanged(const QString &arg1)
{
  // Display new name.
  ui->outputFile->setText(ui->path->text() + QDir::separator() + arg1 +
                          ".qkf");
}

/*******************************************************************************
 * Path Changed.
 ******************************************************************************/
void ProjectProperties::on_path_textChanged(const QString &arg1)
{
  // Display new path.
  ui->outputFile->setText(arg1 + QDir::separator() + ui->name->text() +
                          ".qkf");
}

/*******************************************************************************
 * Location Button clicked.
 ******************************************************************************/
void ProjectProperties::on_location_clicked()
{
  // Set file dialog options.
  QFileDialog directory(this);
  directory.setFileMode(QFileDialog::Directory);
  directory.setOption(QFileDialog::ShowDirsOnly);
  directory.setAcceptMode(QFileDialog::AcceptOpen);
  QString initialPath = QStandardPaths::standardLocations(
                          QStandardPaths::DocumentsLocation)[0] + "/QuickFlow";
  directory.setDirectory(initialPath);
  directory.setWindowTitle("Project path");


  // Execute. If path is selected, change ui->path to set the new directory.
  if (directory.exec())
    ui->path->setText(directory.selectedFiles()[0]);
}

/*******************************************************************************
 * Button Box accepted.
 ******************************************************************************/
void ProjectProperties::on_buttonBox_accepted()
{
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

  // Fill project data.
  project_->name = ui->name->text();
  project_->filePath = ui->path->text() + "/" + project_->name + ".qkf";
  project_->setMaxIterations(ui->maxIterations->text().toUInt());
  project_->setMinError(ui->minError->text().toDouble()/100.0);
  project_->setLengthUn(ui->lengthUnit->currentData().value<Unit::LengthUnit>());
  project_->setImpedanceUn(
    ui->impedanceUnit->currentData().value<Unit::ImpedanceUnit>());
  project_->setVoltageUn(
    ui->voltageUnit->currentData().value<Unit::VoltageUnit>());
  project_->setPowerUn(ui->powerUnit->currentData().value<Unit::PowerUnit>());
  project_->setCurrentUnit(
    ui->currentUnit->currentData().value<Unit::CurrentUnit>());

  // Try to save project.
  bool save_ok = project_->save();

  if (!save_ok) {
    QMessageBox::critical(this, "File write error",
                          "File write error, please choose another location!",
                          QMessageBox::Ok);
    return;
  }

  accept();
}

/*******************************************************************************
 * Button Box rejected.
 ******************************************************************************/
void ProjectProperties::on_buttonBox_rejected()
{
  // If a new project has been created, we need to delete it.
  if(isNew) {
    delete project_;
    project_ = NULL;
  }

  reject();
}

/*!
 * \}
 */

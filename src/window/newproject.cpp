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
 * \file newproject.cpp
 *
 * \brief Implementation of the NewProject class.
 *
 * This file contains the implementation for the NewProject class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#include "newproject.h"
#include "ui_newproject.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QValidator>

#include "customtypes.h"
#include "math_constants.h"

/*******************************************************************************
 * WindowNewProject
 ******************************************************************************/
NewProject::NewProject(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::NewProject)
{
  // Set-up user interface.
  ui->setupUi(this);

  // Create validator for data input.
  ui->maxIterations->setValidator(new QIntValidator(1,1000000, this));
  ui->minError->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->voltageBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->powerBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));

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
}

/*******************************************************************************
 * ~WindowNewProject
 ******************************************************************************/
NewProject::~NewProject()
{
  delete ui;
}

/*******************************************************************************
 * Text Changed.
 ******************************************************************************/
void NewProject::on_name_textChanged(const QString &arg1)
{
  // Display new name.
  ui->outputFile->setText(ui->path->text() + QDir::separator() + arg1 +
                          ".qkf");
}

/*******************************************************************************
 * Path Changed.
 ******************************************************************************/
void NewProject::on_path_textChanged(const QString &arg1)
{
  // Display new path.
  ui->outputFile->setText(arg1 + QDir::separator() + ui->name->text() +
                          ".qkf");
}

/*******************************************************************************
 * Location Button clicked.
 ******************************************************************************/
void NewProject::on_location_clicked()
{
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
void NewProject::on_buttonBox_accepted()
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

  dataName = ui->name->text();
  dataPath = ui->path->text();

  dataMaxIterations = ui->maxIterations->text().toUInt();
  dataMinError = ui->minError->text().toDouble();
  dataVoltageBase = ui->voltageBase->text().toDouble()/kSQRT3;
  dataPowerBase = ui->powerBase->text().toDouble();

  dataLengthUnit = ui->lengthUnit->currentData().value<Unit::LengthUnit>();
  dataImpedanceUnit =
    ui->impedanceUnit->currentData().value<Unit::ImpedanceUnit>();
  dataVoltageUnit = ui->voltageUnit->currentData().value<Unit::VoltageUnit>();
  dataPowerUnit = ui->powerUnit->currentData().value<Unit::PowerUnit>();
  dataCurrentUnit = ui->currentUnit->currentData().value<Unit::CurrentUnit>();

// Return to parent window.
  accept();
}

/*******************************************************************************
 * Button Box rejected.
 ******************************************************************************/
void NewProject::on_buttonBox_rejected()
{
// Return to parent window.
  reject();
}

/*!
 * \}
 */

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
 * \file newproject.h
 *
 * \brief NewProject class definition.
 *
 * This file defines the NewProject class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_NEWPROJECT_H
#define WINDOW_NEWPROJECT_H

#include <QDialog>
#include <QString>
#include <QDir>

#include "customtypes.h"

namespace Ui
{
class NewProject;
}

/*!
 * \class NewProject
 * \brief  Gets user input to create a new project.
 *
 * ### Overview
 * This class implements a dialog that will get user input to create a new
 * project. All parameters are validade for their respective type.
 *
 * In order to verify user confirmation, check for the QDialog::Accepted
 * response.
 *
 * User input is passed through the data* properties.
 *
 * ### Example
 * \code
 * NewProject newProject(this);
 *
 * // If canceled return
 * if (newProject.exec() != QDialog::Accepted) {
 *   return;
 * }
 *
 * // Create project object and fill settings.
 * Project *project = new Project;
 * project->name = newProject.dataName;
 * project->filePath = newProject.dataPath + QDir::separator() +
 *                     newProject.dataName + ".qkf";
 *
 * // Simulation data.
 * project->setMaxIterations(newProject.dataMaxIterations);
 * project->setMinError(newProject.dataMinError);
 * project->setVoltageBase(newProject.dataVoltageBase);
 * project->setPowerBase(newProject.dataPowerBase);
 * \endcode
 */
class NewProject : public QDialog
{
  Q_OBJECT

public:
  /*****************************************************************************
  * Properties.
  *****************************************************************************/
  /*!
   * \brief Project name.
   */
  QString dataName;

  /*!
   * \brief Project path.
   */
  QString dataPath;

  /*!
   * \brief Max. iterations.
   */
  u_int32_t dataMaxIterations;

  /*!
   * \brief Min. error, in percentage.
   */
  double dataMinError;

  /*!
   * \brief Voltage base, in volts.
   */
  double dataVoltageBase;

  /*!
   * \brief Power base, in VA.
   */
  double dataPowerBase;

  /*!
   * \brief Length unit.
   */
  Unit::LengthUnit dataLengthUnit;

  /*!
   * \brief Impedance unit.
   */
  Unit::ImpedanceUnit dataImpedanceUnit;

  /*!
   * \brief Voltage unit.
   */
  Unit::VoltageUnit dataVoltageUnit;

  /*!
   * \brief Power unit.
   */
  Unit::PowerUnit dataPowerUnit;

  /*!
   * \brief Current unit.
   */
  Unit::CurrentUnit dataCurrentUnit;

  /*****************************************************************************
  * Constructor.
  *****************************************************************************/
  /*!
   * \brief NewProject constructor.
   *
   * \param[in] parent Parent widget.
   */
  explicit NewProject(QWidget *parent = 0);

  /*****************************************************************************
  * Destructor.
  *****************************************************************************/
  /*!
   * \brief Destructor.
   * Destroy user interface.
   */
  ~NewProject();

private slots:
  /*****************************************************************************
  * Name text changed..
  *****************************************************************************/
  /*!
   * \brief Handles a project name change.
   * Adjust the file path line edit to conform with the new name.
   *
   * \param[in] arg1 New project name.
   */
  void on_name_textChanged(const QString &arg1);

  /*****************************************************************************
  * Path text change.
  *****************************************************************************/
  /*!
   * \brief Handles text changes in the path line edit.
   * Adjust the file path line edit to conform with the new path.
   *
   * \param[in] arg1 New path.
   */
  void on_path_textChanged(const QString &arg1);


  /*****************************************************************************
  * Location button clicked.
  *****************************************************************************/
  /*!
   * \brief Search the project save directory.
   * Open the path search dialog to select the output directory.
   */
  void on_location_clicked();

  /*****************************************************************************
  * Accepted.
  *****************************************************************************/
  /*!
   * \brief User accepted.
   * Validade all data and fill the data* fields.
   */
  void on_buttonBox_accepted();

  /*****************************************************************************
  * Rejected.
  *****************************************************************************/
  /*!
   * \brief User caceled.
   * User pressed the cancel button.
   */
  void on_buttonBox_rejected();

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief User interface.
   */
  Ui::NewProject *ui;
};

#endif  // WINDOW_NEWPROJECT_H

/*!
 * \}
 */

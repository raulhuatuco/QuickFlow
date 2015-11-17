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
 * \file projectproperties.h
 *
 * \brief ProjectProperties class definition.
 *
 * This file defines the ProjectProperties class.
 *
 * \author David Krepsky
 * \version 0.3
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_PROJECTPROPERTIES_H
#define WINDOW_PROJECTPROPERTIES_H

#include <cstdint>
#include <QDialog>
#include <QString>
#include <QDir>
#include "project.h"
#include "customtypes.h"

namespace Ui
{
class ProjectProperties;
}

/*!
 * \class ProjectProperties
 * \brief  Gets user input to create or edit a project.
 *
 * ### Overview
 * This class implements a dialog that will get user input to create or edit a
 * project. All parameters are validated for their respective type.
 *
 * In order to verify user confirmation, check for the QDialog::Accepted
 * response.
 *
 */
class ProjectProperties : public QDialog
{
  Q_OBJECT

public:
  /*****************************************************************************
  * Public methods.
  *****************************************************************************/
  /*!
   * \brief ProjectProperties constructor.
   *
   * \param[in] parent Parent widget.
   */
  explicit ProjectProperties(QWidget *parent = 0);

  /*!
   * \brief Destructor.
   * Destroy user interface.
   */
  ~ProjectProperties();

  /*!
   * \brief Set the project to be edited.
   *
   * To create a new project, pass NULL as the parameter.
   *
   * \param[in,out] project Project to be edited. If NULL, a new project is
   * created.
   */
  void setOptions(Project *project);

  /*!
   * \brief Project being edited.
   * \return Address of the project being edited.
   */
  Project *project();

private slots:
  /*****************************************************************************
  * Slots.
  *****************************************************************************/
  /*!
   * \brief Handles a project name change.
   * Adjust the file path line edit to conform with the new name.
   *
   * \param[in] arg1 New project name.
   */
  void on_name_textChanged(const QString &arg1);

  /*!
   * \brief Handles text changes in the path line edit.
   * Adjust the file path line edit to conform with the new path.
   *
   * \param[in] arg1 New path.
   */
  void on_path_textChanged(const QString &arg1);

  /*!
   * \brief Search the project save directory.
   * Open the path search dialog to select the output directory.
   */
  void on_location_clicked();

  /*!
   * \brief User accepted.
   * Validade all data and fill the data* fields.
   */
  void on_buttonBox_accepted();

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
  Ui::ProjectProperties *ui;

  /*!
   * \brief Project to be edited.
   */
  Project *project_;

  /*!
   * \brief True if a new project has been created.
   */
  bool isNew;
};

#endif  // WINDOW_PROJECTPROPERTIES_H

/*!
 * \}
 */

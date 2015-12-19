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
 * \file quickflow.h
 *
 * \brief Main window class definition.
 *
 * This file defines the QuickFlow class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 10/2015
 * \copyright David Krepsky
 */

#ifndef QUICKFLOW_H
#define QUICKFLOW_H

#include <QtWidgets>
#include <QtCore>
#include <QtGui>

#include "project.h"

namespace Ui
{
class QuickFlow;
}

/*!
 * \class QuickFlow
 * \brief This is the main window of the software.
 *
 * ### Overview
 * QuickFlow class provides the main window for the Quick Flow software.
 * It provides the work environment with all the menus, buttons and a graphics
 * view.
 *
 * This class is responsible for creating a project and editing
 * \warning One, and only one instance of this class must be created.
 */
class QuickFlow : public QMainWindow
{
  Q_OBJECT

public:
  /*****************************************************************************
  * Const.
  *****************************************************************************/
  static const QString kVersion;

  /*****************************************************************************
  * Constructor.
  *****************************************************************************/
  /*!
   * \brief Constructor.
   * Here, we must initialize the user interface and load the software settings,
   * updating or creating a new set of configurantion if needed.
   *
   * \param[in] parent Parent widget.
   */
  explicit QuickFlow(QWidget *parent = 0);

  /*****************************************************************************
  * Destructor.
  *****************************************************************************/
  /*!
   * \brief Destructor.
   * Must delete the user interface.
   */
  ~QuickFlow();


  /*****************************************************************************
  * Is altered.
  *****************************************************************************/
  /*!
   * \brief Check if project is altered
   * Check the altered flag.
   * This flag is cleared everytime the project is saved and is set when any
   * alteration occurs.
   *
   * \return true if project has been altered, false otherwise.
   *
   * \note Must be replaced by Qt undo framework.
   */
  bool isAltered();

  /*****************************************************************************
  * Set altered.
  *****************************************************************************/
  /*!
   * \brief Set the altered flag.
   * If \b altered is true, enable the save action and set altered_ to
   * true. If false, disable the save action.
   *
   * \param[in] altered Must be replaced by Qt undo framework.
   */
  void setAltered(bool altered);

private slots:

  /*****************************************************************************
  * Zoom in action.
  *****************************************************************************/
  /*!
   * \brief Zoom into the view.
   */
  void on_actionZoomIn_triggered();

  /*****************************************************************************
  * Zoom out action.
  *****************************************************************************/
  /*!
   * \brief Zoom out the view.
   */
  void on_actionZoomOut_triggered();

  /*****************************************************************************
  * Zoom fit action.
  *****************************************************************************/
  /*!
   * \brief Fit the entire system into the view.
   */
  void on_actionZoomFit_triggered();

  /*****************************************************************************
  * Exit action.
  *****************************************************************************/
  /*!
   * \brief Close the software.
   */
  void on_actionExit_triggered();

  /*****************************************************************************
  * New project action.
  *****************************************************************************/
  /*!
   * \brief Opens the new project window.
   * Get user input to create a new project.
   */
  void on_actionNew_triggered();

  /*****************************************************************************
  * Open project action.
  *****************************************************************************/
  /*!
   * \brief Load a project from a .qkf file.
   */
  void on_actionOpen_triggered();

  /*****************************************************************************
  * Save project action.
  *****************************************************************************/
  /*!
   * \brief Save the project.
   * This action will only be avaliable when there are changes to the project.
   */
  void on_actionSave_triggered();

  /*****************************************************************************
  * Save project as action.
  *****************************************************************************/
  /*!
   * \brief Saves project to another file.
   */
  void on_actionSave_as_triggered();

  /*****************************************************************************
  * Close project action.
  *****************************************************************************/
  /*!
   * \brief Closes the current project.
   */
  void on_actionClose_triggered();

  /*****************************************************************************
  * Add network action.
  *****************************************************************************/
  /*!
   * \brief Add a new network.
   * Each network represents a different allimentator.
   */
  void on_actionAdd_Network_triggered();

  /*****************************************************************************
  * Add bar action.
  *****************************************************************************/
  /*!
   * \brief Open the bar properties window to create a new bar.
   */
  void on_actionAddBar_triggered();

  /*****************************************************************************
  * Add line action.
  *****************************************************************************/
  /*!
   * \brief Open the line properties window to create a new line.
   */
  void on_actionAddLine_triggered();

  /*****************************************************************************
  * Import txt type 1 action.
  *****************************************************************************/
  /*!
   * \brief Import a network from a text file that was formated in a "type 1"
   * manner.
   * Type 1 formatin is used by the 14 bars system in "documentation/raw data"
   * folder.
   */
  void on_action_txt_type_1_triggered();

  /*****************************************************************************
  * Import txt type 2 action.
  *****************************************************************************/
  /*!
   * \brief Import a network from a text file that was formated in a "type 2"
   * manner.
   * Type 2 formatin is used by the A1 system in "documentation/raw data"
   * folder.
   */
  void on_action_txt_type_2_triggered();

  /*****************************************************************************
  * Import txt type 3 action.
  *****************************************************************************/
  /*!
   * \brief Import a network from a text file that was formated in a "type 3"
   * manner.
   * Type 3 formatin is used by the A2 system in "documentation/raw data"
   * folder.
   */
  void on_action_txt_type_3_triggered();

  /*****************************************************************************
  * Cespedes action.
  *****************************************************************************/
  /*!
   * \brief Select the Cespedes algorithm.
   *
   * \note Change this to the right algorithm name.
   */
  void on_actionCespedes_triggered();

  /*****************************************************************************
  * Shirmoharmmadi action.
  *****************************************************************************/
  /*!
   * \brief Select the Shirmoharmmadi algorithm.
   *
   * \note Chage this to the right algorithm name.
   */
  void on_actionShirmoharmmadi_triggered();

  /*****************************************************************************
  * Sugiyama graph algorithm action.
  *****************************************************************************/
  /*!
   * \brief Redraw the network using the Sugiyama algorithm.
   */
  void on_actionSugiyama_triggered();

  /*****************************************************************************
  * Sugiyama fast graph algorithm action.
  *****************************************************************************/
  /*!
   * \brief Redraw the network using an faster Sugiyama algorithm (worst
   * euristics).
   */
  void on_actionSugiyama_Fast_triggered();

  /*****************************************************************************
  * Multi-level graph algorithm action.
  *****************************************************************************/
  /*!
   * \brief Redraw the network using the multi-level algorithm.
   */
  void on_actionMulti_level_triggered();

  /*****************************************************************************
  * Search bar action.
  *****************************************************************************/
  /*!
   * \brief Search a bar and give it focus.
   */
  void on_actionSearch_Bar_triggered();

  /*****************************************************************************
  * Run action.
  *****************************************************************************/
  /*!
   * \brief Compute the power flow.
   */
  void on_actionRun_triggered();

  /*****************************************************************************
  * About action.
  *****************************************************************************/
  /*!
   * \brief About information.
   */
  void on_actionAbout_triggered();

  /*****************************************************************************
   * Edit bar.
   ****************************************************************************/
  /*!
   * \brief Opens the bar properties editor for bar \b bar.
   * This slot handles the bar double click event.
   *
   * \param[in/out] bar Bar to be edited.
   */
  void on_editBar(QObject *bar);

  /*****************************************************************************
   * Edit line.
   ****************************************************************************/
  /*!
   * \brief Opens the line properties editor for line \b line.
   * This slot handles the line double click event.
   *
   * \param[in/out] line Line to be edited.
   */
  void on_editLine(QObject *line);

  void on_actionSettings_triggered();
  
  void on_actionExport_image_triggered();
  
  void on_actionExport_triggered();
  
protected:
  /*****************************************************************************
  * Close event.
  *****************************************************************************/
  /*!
   * \brief Handles software termination.
   * This event will check if there is something to be saved and, if so, confirm
   * the user will to save or discard the changes.
   */
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief Used interface.
   * Automatically generated class for the user interface.
   */
  Ui::QuickFlow *ui;

  /*!
   * \brief Software settings.
   * Used to save general settings, like theme, app version and etc.
   * Check constructor for detais.
   */
  QSettings *settings;

  /*!
   * \brief Quick flow project.
   * Pointer to the opened project.
   *
   * \note Add functionality to work on multiple projects simultaneously.
   */
  Project *project;

  /*!
   * \brief Altered flag.
   * True if user made any changes to the current project. False if no changes
   * where made since the last save.
   */
  bool altered_;

  /*****************************************************************************
  * Load application settings.
  *****************************************************************************/
  /*!
   * \brief Load application settings.
   * Used to load themes, version and etc.
   *
   * \note Check implementation for details.
   */
  void loadSettings();

  /*****************************************************************************
  * Save application settings.
  *****************************************************************************/
  /*!
   * \brief Save application settings.
   * Save the app settings using QSettings class.
   */
  void saveSettings();

  /*****************************************************************************
  * Create application settings.
  *****************************************************************************/
  /*!
   * \brief Create application settings.
   * Used when the software is used for the first time.
   */
  void createSettings();

  /*****************************************************************************
  * Upgrade application settings.
  *****************************************************************************/
  /*!
   * \brief Upgrade application settings.
   * Used when installing a new version of the software.
   */
  void upgradeSettings();

  /*****************************************************************************
   * No project interface.
   *****************************************************************************/
  /*!
   * \brief Disable all project related actions.
   * Will disable any action that depens on projects.
   */
  void noProjectInterface();

  /*****************************************************************************
  * Work interface.
  *****************************************************************************/
  /*!
   * \brief Enable all project related actions.
   * Used to enable the interface after a project is created or opened.
   */
  void workInterface();

  /*****************************************************************************
  * connectSignals.
  *****************************************************************************/
  void connectSignals();

  /*****************************************************************************
  * disconnectSignals.
  *****************************************************************************/
  void disconnectSignals();
};

#endif  // QUICKFLOW_H

/*!
 * \}
 */

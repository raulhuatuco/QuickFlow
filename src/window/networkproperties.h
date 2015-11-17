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
 * \file networkproperties.h
 *
 * \brief NetworkProperties class definition.
 *
 * This file defines the NetworkProperties class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_NETOWRKPROPERTIES_H
#define WINDOW_NETOWRKPROPERTIES_H

#include <QDialog>
#include "project.h"
#include "models/network.h"

namespace Ui
{
class NetworkProperties;
}

/*!
 * \class NetworkProperties
 * \brief  Gets user input to create or edit a network.
 *
 * ### Overview
 * This class implements a dialog that will get user input to create or edit a
 * network. All parameters are validated for their respective type.
 *
 * In order to verify user confirmation, check for the QDialog::Accepted
 * response.
 *
 * To create a new network, use setOptions(project, NULL).
 *
 * Note that we use the Style Sheets feature from Qt in the color pickers.
 * This was a decision based on the fact that there is no other easy way to
 * setup a color picker button.
 */
class NetworkProperties : public QDialog
{
  Q_OBJECT

public:
  /*****************************************************************************
  * Public methods.
  *****************************************************************************/
  /*!
   * \brief Class constructor.
   * \param[in] parent Parent widget.
   */
  explicit NetworkProperties(QWidget *parent = 0);

  /*!
   * \brief Destructor.
   */
  ~NetworkProperties();

  /*!
   * \brief Set edit options.
   *
   * \param[in] project Project in use.
   * \param[in,out] network Network to be edited. To create a new one, pass NULL
   * to this parameter.
   */
  void setOptions(Project *project, Network *network);

  /*!
    * \brief Network used.
    *
    * \return Pointer to the nerwork edited/created.
    */
  Network *network();

  /*****************************************************************************
  * Slots.
  *****************************************************************************/
private slots:
  /*!
   * \brief Slack bar color selection box clicked;
   */
  void on_slackColor_clicked();

  /*!
   * \brief PQ color selection box clicked.
   */
  void on_pqColor_clicked();

  /*!
   * \brief Bar border color selection box clicked.
   */
  void on_barBorderColor_clicked();

  /*!
   * \brief Line color selection box clicked.
   */
  void on_lineColor_clicked();

  /*!
   * \brief Changes accepted.
   */
  void on_buttonBox_accepted();

  /*!
   * \brief Changes rejected.
   */
  void on_buttonBox_rejected();

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief User interface.
   */
  Ui::NetworkProperties *ui;

  /*!
   * \brief Project being edited/created.
   */
  Project *project_;

  /*!
   * \brief Network being edited/created.
   */
  Network *network_;

  /*!
   * \brief Mark if a network was created.
   */
  bool isNew;

  /*!
   * \brief Holds slack bar color.
   */
  QColor slack;

  /*!
   * \brief Holds PQ bar color.
   */
  QColor pq;

  /*!
   * \brief Holds bar border color.
   */
  QColor barBorder;

  /*!
   * \brief Holds line color.
   */
  QColor line;
};

#endif // WINDOW_NETOWRKPROPERTIES_H

/*!
 * \}
 */

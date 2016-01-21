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
 * \file about.h
 *
 * \brief About class definition.
 *
 * This file defines the About window class.
 *
 * \author David Krepsky
 * \version 0.3
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_ABOUT_H
#define WINDOW_ABOUT_H

#include <QDialog>

namespace Ui
{
class about;
}

/*!
 * \class About
 * \brief Window to display information about the software.
 *
 * ### Overview
 * Contains information about the author and the software.
 * Should be linked to an action (about action).
 */
class About : public QDialog
{
  Q_OBJECT

public:
  /*****************************************************************************
  * Public methods.
  *****************************************************************************/
  /*!
   * \brief Constructor.
   * Create the class.
   *
   * \param[in] parent Parent widget.
   */
  explicit About(QWidget *parent = 0);

  /*!
   * \brief Destructor.
   * Destroy user interface.
   */
  ~About();

private slots:
  /*****************************************************************************
  * Slots.
  *****************************************************************************/
  /*!
   * \brief Ok button.
   * Close this window.
   */
  void on_ok_clicked();

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief User interface.
   */
  Ui::about *ui;
};

#endif // ABOUT_H

/*!
 * \}
 */
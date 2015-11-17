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
 * \file searchbar.h
 *
 * \brief SearchBar class definition.
 *
 * This file defines the SearchBar class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_SEARCHBAR_H
#define WINDOW_SEARCHBAR_H

#include <QDialog>
#include "project.h"
#include "models/bar.h"

namespace Ui
{
class SearchBar;
}

/*!
 * \class SearchBar
 * \brief Window to search for a bar inside a network.
 *
 * ### Overview
 * SearchBar class provides a window that will select the bar to search.
 *
 * If accepted and bar exists, bar() will return the pointer to the searched
 * bar.
 */
class SearchBar : public QDialog
{
  Q_OBJECT

  /*****************************************************************************
  * Public methods.
  *****************************************************************************/
public:
  /*!
   * \brief Constructor.
   *
   * \param[in] parent Parent widget.
   */
  explicit SearchBar(QWidget *parent = 0);

  /*!
   *  \brief Destructor.
   */
  ~SearchBar();

  /*!
   * \brief Set the project where the networks resides.
   *
   * \param[in] project Project that contains the networks to be searched.
   */
  void setOptions(Project *project);

  /*!
   * \brief Return the bar found.
   *
   * Will always return a valid pointer.
   *
   * \return Pointer to the bar found.
   */
  Bar *bar();

  /*****************************************************************************
  * Slots.
  *****************************************************************************/
private slots:
  /*!
   * \brief Search button clicked.
   */
  void on_search_clicked();

  /*!
   * \brief Cancel button clicked.
   */
  void on_cancel_clicked();

private:
  /*****************************************************************************
  * Private methods.
  *****************************************************************************/
  /*!
   * \brief User interface.
   */
  Ui::SearchBar *ui;

  /*!
   * \brief Project under use.
   */
  Project *project_;

  /*!
   * \brief Pointer to the searched bar.
   */
  Bar *bar_;
};

#endif  // WINDOW_SEARCHBAR_H

/*!
 * \}
 */

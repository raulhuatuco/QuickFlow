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
 * If accepted, id() gives the bar id to search for and network() gives the
 * network pointer.
 */
class SearchBar : public QDialog
{
  Q_OBJECT

public:
  explicit SearchBar(QWidget *parent = 0);

  ~SearchBar();

  void setOptions(Project *project);

  Bar *bar();

private slots:
  void on_search_clicked();
  
  void on_cancel_clicked();
  
private:
  Ui::SearchBar *ui;

  Project *project_;

  Bar *bar_;
};

#endif  // WINDOW_SEARCHBAR_H

/*!
 * \}
 */

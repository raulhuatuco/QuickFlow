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
 * \version 0.1
 * \date 10/2015
 * \copyright David Krepsky
 */

#ifndef WINDOW_SEARCHBAR_H
#define WINDOW_SEARCHBAR_H

#include <QDialog>
#include "project.h"

namespace Ui
{
class SearchBar;
}

/*!
 * \class SearchBar
 * \brief Window to search for a bar inside a network.
 *
 * ### Overview
 * After creating a new network or importing one, it is necessary to make it
 * have a "good look". This is hard to do by hand for systems composed of
 * thousands of bars and lines. In order to automate the repositioning of
 * elements in an organised and clean way, the RedrawNetwork class provides some
 * graph drawing algorithms that makes all the needed calculation in order to
 * rearrange the network elements.
 *
 * The following graph drawing algorithms are present:
 *  - Sugiyama: Classical Sugiyama algorithm for layered graphs.
 *  - SugiyamaFast: Same as above, but with a faster method.
 *  - Multilevel: Usefull to give a nice view of how the system expands.
 *
 * This class uses the Open Graphics Drawing Framework (OGDF) to provide its
 * functionalities.
 * Please make sure to have ogdf and coin binaries if compilled with dynamic
 * link.
 * More info about OGDF: http://www.ogdf.net/doku.php
 *
 * ### Example
 * \code
 * // Code to redraw a network (network is a pointer to a Network object).
 *  RedrawNetwork redraw(network);
 *  redraw.sugiyama();
 *
 * \endcode
 *
 * \todo Improve removeOffset to consider slack initial position.
 * \bug Can't delete Graph or GA objects.
 *
 */
class SearchBar : public QDialog
{
  Q_OBJECT

public:
  explicit SearchBar(QWidget *parent = 0);
  ~SearchBar();

  void setOptions(Project *project);

private:
  Ui::SearchBar *ui;
  Project *project_;

};

#endif  // WINDOW_SEARCHBAR_H
/*!
 * \}
 */

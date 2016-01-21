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
 * \addtogroup Algorithms
 * \{
 */

/*!
 * \file redrawnetwork.h
 *
 * \brief RedrawNetwork class definition.
 *
 * This file defines the RedrawNetwork class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 11/2015
 * \copyright David Krepsky
 */

#ifndef ALGORITHMS_REDRAW_H
#define ALGORITHMS_REDRAW_H

#include <QMap>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include "models/bar.h"
#include "models/line.h"
#include "models/network.h"

QT_BEGIN_NAMESPACE
class Bar;
class Line;
class Network;
QT_END_NAMESPACE

/*!
 * \class RedrawNetwork
 * \brief Algorithm that reposition every element in a network.
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
class RedrawNetwork
{
public:
  /*****************************************************************************
  * Constructor.
  *****************************************************************************/
  /*!
   * \brief Class constructor.
   * A valid pointer to a Network object is needed.
   *
   * \param[in] network Network to be redrawn.
   */
  RedrawNetwork(Network *network);

  /*****************************************************************************
  * Destructor.
  *****************************************************************************/
  /*!
   * \brief Class destructor.
   * Free used memory.
   */
  ~RedrawNetwork();

  /*****************************************************************************
  * Sugiyama Fast algorithm.
  *****************************************************************************/
  /*!
   * \brief Redraws using Sugiyama Fast algorithm.
   * This will draw faster than the Sugiyama, but with a lower quality result.
   */
  void sugiyamaFast();

  /*****************************************************************************
  * Sugiyama algorithm.
  *****************************************************************************/
  /*!
   * \brief Redraws using Sugiyama algorithm.
   * Really nice looking for radial networks.
   */
  void sugiyama();

  /*****************************************************************************
  * Multilevel algorithm.
  *****************************************************************************/
  /*!
   * \brief Redraws with Multilevel algorithm.
   * Provides a nice way to see how a system expands from the slack bar.
   */
  void multiLevel();

private:
  /*****************************************************************************
  * Private data.
  *****************************************************************************/
  /*!
   * \brief Pointer to the network that will be redrawn.
   */
  Network *network_;

  /*!
   * \brief OGDF Graph object.
   */
  ogdf::Graph *graph;

  /*!
   * \brief OGDF GraphAttributes object.
   */
  ogdf::GraphAttributes *graphAttributes;

  /*!
   * \brief QMap to link bars to ogdf nodes.
   * This was the way I found to link a node in an OGDF Graph to a bar in a
   * Network.
   */
  QMap<int, ogdf::node> nodeMap;

  /*****************************************************************************
  * Create OGDF graph.
  *****************************************************************************/
  /*!
   * \brief Creates the graph structure and attributes based on network_.
   */
  void createGraph();

  /*****************************************************************************
  * Apply Changes.
  *****************************************************************************/
  /*!
   * \brief Alter bars position.
   */
  void applyChanges();

  /*****************************************************************************
  * Remove Offset.
  *****************************************************************************/
  /*!
   * \brief Remove position offset from OGDF.
   */
  void removeOffset();

};

#endif // ALGORITHMS_REDRAW_H

/*!
 * \}
 */

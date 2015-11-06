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
 * \file redrawnetwork.cpp
 *
 * \brief Implementation of RedrawNetwork class.
 *
 * This is the implementation of the RedrawNetwork class.
 *
 * \author David Krepsky
 * \version 0.1
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "redrawnetwork.h"
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/layered/BarycenterHeuristic.h>
#include <ogdf/energybased/MultilevelLayout.h>
#include <ogdf/misclayout/BertaultLayout.h>

using namespace ogdf;

/*******************************************************************************
* Constructor.
*******************************************************************************/
RedrawNetwork::RedrawNetwork(Network *network) :
  network_(network),
  graph(NULL),
  graphAttributes(NULL)
{
}

/*******************************************************************************
* Destructor.
*******************************************************************************/
RedrawNetwork::~RedrawNetwork()
{
// Cant delete graph object, can't alloc at stack neither.
// A dark bug that will crash the app.
// Need fix.
//  if(graph) {
//    graph->clear();
//    delete graph;
//  }

//  if(graphAttributes)
//    delete graphAttributes;
}

/*******************************************************************************
* Sugiyama Fast algorithm.
*******************************************************************************/
void RedrawNetwork::sugiyamaFast()
{
  createGraph();

  SugiyamaLayout SL;
  SL.setRanking(new OptimalRanking);
  SL.setCrossMin(new BarycenterHeuristic);
  FastHierarchyLayout *fhl = new FastHierarchyLayout;
  fhl->layerDistance(60.0);    // set layer distance to 40.0
  fhl->nodeDistance(40.0); // set node distance to 20.0.
  SL.setLayout(fhl);
  SL.call(*graphAttributes);

  applyChanges();

  removeOffset();
}

/*******************************************************************************
* Sugiyama algorithm.
*******************************************************************************/
void RedrawNetwork::sugiyama()
{
  createGraph();

  SugiyamaLayout SL;
  SL.setRanking(new OptimalRanking);
  SL.setCrossMin(new MedianHeuristic);
  OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
  ohl->layerDistance(60.0);
  ohl->nodeDistance(40.0);
  SL.setLayout(ohl);
  SL.call(*graphAttributes);

  applyChanges();

  removeOffset();
}

/*******************************************************************************
* Multilevel algorithm.
*******************************************************************************/
void RedrawNetwork::multiLevel()
{
  createGraph();

  MultilevelLayout MLL;
  MLL.call(*graphAttributes);

  applyChanges();

  removeOffset();
}

/*******************************************************************************
* Create OGDF graph.
*******************************************************************************/
void RedrawNetwork::createGraph()
{
  graph = new Graph;
  graphAttributes = new GraphAttributes(*graph, GraphAttributes::nodeGraphics |
                                        GraphAttributes::edgeGraphics |
                                        GraphAttributes::edgeType |
                                        GraphAttributes::edgeArrow);
  graphAttributes->setDirected(true);

  node n;

  foreach(Bar *bar, network_->bars) {
    n = graph->newNode(bar->id);
    nodeMap.insert(bar->id,n);
  }

  forall_nodes(n, *graph) {
    graphAttributes->height(n) = Network::barIconSize;
    graphAttributes->width(n) = Network::barIconSize;
    graphAttributes->shape(n) = shRect;
  }

  foreach(Line *line, network_->lines) {
    node noI = nodeMap.value(line->nodes.first);
    node noF = nodeMap.value(line->nodes.second);
    graph->newEdge(noI, noF);
  }

  edge e;
  forall_edges(e, *graph) {
    graphAttributes->arrowType(e) = eaNone;
  }
}

/*******************************************************************************
* Apply changes.
*******************************************************************************/
void RedrawNetwork::applyChanges()
{
  node n;

  forall_nodes(n, *graph) {  // iterate through all the nodes in the graph.
    double x = graphAttributes->x(n);
    double y = graphAttributes->y(n);
    int id = n->index();
    Bar *bar = network_->getBarById(id);

    bar->setPos(y, x);
  }
}

/*******************************************************************************
* Remove Offset.
*******************************************************************************/
void RedrawNetwork::removeOffset()
{
  Bar *slack = network_->getBarById(0);
  double xOffset = slack->x();
  double yOffset = slack->y();
  slack->setX(0);
  slack->setY(0);

  foreach(Bar *bar, network_->bars) {
    if(bar->id != 0) {
      bar->setX(bar->x() - xOffset);
      bar->setY(bar->y() - yOffset);
    }
  }
}

/*!
 * \}
 */

#include "redraw.h"

#include <ogdf/basic/Graph.h>
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
 * redrawGraph1.
 ******************************************************************************/
void redrawSugiyamaFast(Network *network)
{
  Graph G;
  QMap<int, node> nodeMap;

  GraphAttributes GA(
    G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
    GraphAttributes::nodeLabel | GraphAttributes::nodeStyle |
    GraphAttributes::edgeType | GraphAttributes::edgeArrow |
    GraphAttributes::edgeStyle);  // Create graph attributes for this graph

  node n;

  foreach(Bar *bar, network->bars) {
    n = G.newNode(bar->id);
    nodeMap.insert(bar->id,n);
  }

  forall_nodes(n, G) {
    GA.height(n) = Network::barIconSize;
    GA.width(n) = Network::barIconSize;
    GA.shape(n) = shRect;
  }

  foreach(Line *line, network->lines) {
    node noI = nodeMap.value(line->nodes.first);
    node noF = nodeMap.value(line->nodes.second);
    G.newEdge(noI, noF);
  }

  edge e;
  forall_edges(e, G) {
    GA.bends(e);
    GA.arrowType(e) = eaNone;
  }

  SugiyamaLayout SL;
  SL.setRanking(new OptimalRanking);
  SL.setCrossMin(new BarycenterHeuristic);
  FastHierarchyLayout *fhl = new FastHierarchyLayout;
  fhl->layerDistance(40.0);    // set layer distance to 40.0
  fhl->nodeDistance(20.0); // set node distance to 20.0.
  SL.setLayout(fhl);
  SL.call(GA);

  forall_nodes(n, G) {  // iterate through all the node in the graph
    double x = GA.x(n);
    double y = GA.y(n);
    int id = n->index();
    Bar *bar = network->getBarById(id);

    if(bar == NULL) {
      while(1) {}
    }

    bar->setPos(y, x);
  }
}

/*******************************************************************************
 * redrawGraph2.
 ******************************************************************************/
void redrawSugiyama(Network *network)
{
  Graph G;
  QMap<int, node> nodeMap;

  GraphAttributes GA(
    G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
    GraphAttributes::nodeLabel | GraphAttributes::nodeStyle |
    GraphAttributes::edgeType | GraphAttributes::edgeArrow |
    GraphAttributes::edgeStyle);  // Create graph attributes for this graph

  node n;
  foreach(Bar *bar, network->bars) {
    n = G.newNode(bar->id);
    nodeMap.insert(bar->id,n);
  }

  forall_nodes(n, G) {
    GA.height(n) = Network::barIconSize;
    GA.width(n) = Network::barIconSize;
    GA.shape(n) = shRect;
  }

  foreach(Line *line, network->lines) {
    node noI = nodeMap.value(line->nodes.first);
    node noF = nodeMap.value(line->nodes.second);
    G.newEdge(noI, noF);
  }

  edge e;
  forall_edges(e, G) {
    GA.bends(e);
    GA.arrowType(e) = eaNone;
  }

  SugiyamaLayout SL;
  SL.setRanking(new OptimalRanking);
  SL.setCrossMin(new MedianHeuristic);
  OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
  SL.setLayout(ohl);
  SL.call(GA);

  forall_nodes(n, G) {  // iterate through all the node in the graph
    double x = GA.x(n);
    double y = GA.y(n);
    int id = n->index();
    Bar *bar = network->getBarById(id);

    if(bar == NULL) {
      while(1) {}
    }

    bar->setPos(y, x);
  }
}

/*******************************************************************************
 * redrawGraph3.
 ******************************************************************************/
void redrawMultiLevel(Network *network)
{
  Graph G;
  QMap<int, node> nodeMap;

  GraphAttributes GA(
    G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
    GraphAttributes::nodeLabel | GraphAttributes::nodeStyle |
    GraphAttributes::edgeType | GraphAttributes::edgeArrow |
    GraphAttributes::edgeStyle);  // Create graph attributes for this graph

  node n;
  foreach(Bar *bar, network->bars) {
    n = G.newNode(bar->id);
    nodeMap.insert(bar->id,n);
  }

  forall_nodes(n, G) {
    GA.height(n) = Network::barIconSize;
    GA.width(n) = Network::barIconSize;
    GA.shape(n) = shRect;
  }

  foreach(Line *line, network->lines) {
    node noI = nodeMap.value(line->nodes.first);
    node noF = nodeMap.value(line->nodes.second);
    G.newEdge(noI, noF);
  }

  edge e;
  forall_edges(e, G) {
    GA.bends(e);
    GA.arrowType(e) = eaNone;
  }

  MultilevelLayout MLL;
  MLL.call( GA );

  forall_nodes(n, G) {  // iterate through all the node in the graph
    double x = GA.x(n);
    double y = GA.y(n);
    int id = n->index();
    Bar *bar = network->getBarById(id);

    if(bar == NULL) {
      while(1) {}
    }

    bar->setPos(x, y);
  }
}

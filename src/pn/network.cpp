#include "network.h"

#include <QStack>

/*******************************************************************************
 * PnNetwork.
 ******************************************************************************/
Network::Network() :
  perUnit(false)
{
  barDoubleClick = new QSignalMapper(this);
  lineDoubleClick = new QSignalMapper(this);

  // Create mapper for bar properties.
  connect(barDoubleClick, SIGNAL(mapped(QObject *)), this,
          SIGNAL(barProperties(QObject *)));

  // Create mapper for line properties.
  connect(lineDoubleClick, SIGNAL(mapped(QObject *)), this,
          SIGNAL(lineProperties(QObject *)));
}

/*******************************************************************************
 * ~PnNetwork.
 ******************************************************************************/
Network::~Network() {}

/*******************************************************************************
 * addBar.
 ******************************************************************************/
bool Network::addBar(Bar *bar)
{
  if(getBarById(bar->id) != NULL)
    return false;

  bar->pnNetwork = this;
  addItem(bar);
  bars.insert(bar->id, bar);

  connect(bar, SIGNAL(eventDoubleClick()), barDoubleClick, SLOT(map()));
  barDoubleClick->setMapping(bar, bar);

  return true;
}

/*******************************************************************************
 * addLine.
 ******************************************************************************/
bool Network::addLine(PnLine *line)
{
  if(getLineByNodes(line->noI, line->noF) != NULL)
    return false;

  Bar *pNoI = getBarById(line->noI);
  Bar *pNoF = getBarById(line->noF);

  if ((pNoI == NULL) || (pNoF == NULL))
    return false;

  line->pnNetwork = this;
  line->setNodes(pNoI, pNoF);

  addItem(line);
  lines.append(line);

  connect(line, SIGNAL(eventDoubleClick()), lineDoubleClick, SLOT(map()));
  lineDoubleClick->setMapping(line, line);

  return true;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
Bar *Network::getBarById(uint32_t id)
{
  return bars.value(id, NULL);
}

/*******************************************************************************
 * getLineByNodes.
 ******************************************************************************/
PnLine *Network::getLineByNodes(uint32_t noI, uint32_t noF)
{
  foreach (PnLine *line, lines) {
    if ((line->noI == noI) && (line->noF == noF))
      return line;

    if ((line->noF == noI) && (line->noI == noF))
      return line;
  }

  return NULL;
}

void Network::toPerUnit()
{
  if(perUnit)
    return;


}

void Network::toBaseUnit()
{
  if(!perUnit)
    return;

}

bool Network::isPerUnit()
{
  return perUnit;
}

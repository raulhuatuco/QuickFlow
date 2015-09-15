#include "network.h"

#include <QStack>

#include "pn/layer.h"

/*******************************************************************************
 * PnNetwork.
 ******************************************************************************/
PnNetwork::PnNetwork() :
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
PnNetwork::~PnNetwork() {}

/*******************************************************************************
 * addBar.
 ******************************************************************************/
bool PnNetwork::addBar(PnBar *bar)
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
bool PnNetwork::addLine(PnLine *line)
{
  if(getLineByNodes(line->noI, line->noF) != NULL)
    return false;

  PnBar *pNoI = getBarById(line->noI);
  PnBar *pNoF = getBarById(line->noF);

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
PnBar *PnNetwork::getBarById(uint32_t id)
{
  return bars.value(id, NULL);
}

/*******************************************************************************
 * getLineByNodes.
 ******************************************************************************/
PnLine *PnNetwork::getLineByNodes(uint32_t noI, uint32_t noF)
{
  foreach (PnLine *line, lines) {
    if ((line->noI == noI) && (line->noF == noF))
      return line;

    if ((line->noF == noI) && (line->noI == noF))
      return line;
  }

  return NULL;
}

void PnNetwork::toPerUnit()
{
  if(perUnit)
    return;


}

void PnNetwork::toBaseUnit()
{
  if(!perUnit)
    return;

}

bool PnNetwork::isPerUnit()
{
  return perUnit;
}

/*******************************************************************************
 * redrawAlgorithm1.
 ******************************************************************************/
void PnNetwork::redrawAlgorithm1()
{
  PnLayer layers(PnNetwork);

}

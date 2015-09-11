#include "pnnetwork.h"

/*******************************************************************************
 * PnNetwork.
 ******************************************************************************/
PnNetwork::PnNetwork()
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

  addItem(bar);
  barMap.insert(bar->id, bar);

  connect(bar, SIGNAL(eventDoubleClick()), barDoubleClick, SLOT(map()));
  barDoubleClick->setMapping(bar, bar);

  return true;
}

/*******************************************************************************
 * addLine.
 ******************************************************************************/
bool PnNetwork::addLine(PnLine *line)
{
  if(getLineByNodes(line->noI()->id, line->noF()->id) != NULL)
    return false;

  addItem(line);
  lineVector.append(line);

  connect(line, SIGNAL(eventDoubleClick()), lineDoubleClick, SLOT(map()));
  lineDoubleClick->setMapping(line, line);

  return true;
}

/*******************************************************************************
 * getBarById.
 ******************************************************************************/
PnBar *PnNetwork::getBarById(uint32_t id)
{
  return barMap.value(id, NULL);
}

/*******************************************************************************
 * getLineByNodes.
 ******************************************************************************/
PnLine *PnNetwork::getLineByNodes(uint32_t noI, uint32_t noF)
{
  foreach (PnLine *line, lineVector) {
    uint32_t NoIid = line->noI()->id;
    uint32_t NoFid = line->noF()->id;

    if ((NoIid == noI) || (NoIid == noF))
      if ((NoFid == noI) || (NoFid == noF))
        return line;
  }
  return NULL;
}

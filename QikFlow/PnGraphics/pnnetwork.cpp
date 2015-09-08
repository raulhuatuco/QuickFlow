#include "pnnetwork.h"

PnNetwork::PnNetwork() {}

PnNetwork::~PnNetwork() {}

bool PnNetwork::addBar(PnBar *bar) {
  addItem(bar);
  barMap.insert(bar->id, bar);
  return true;
}

bool PnNetwork::addLine(PnLine *line) {
  addItem(line);
  lineIMap.insert(line->noI()->id, line);
  lineFMap.insert(line->noF()->id, line);
  return true;
}

PnBar *PnNetwork::getBarById(uint32_t id) {
  return barMap.value(id, NULL);
}

PnLine *PnNetwork::getLineByNodes(uint32_t noI, uint32_t noF) {
  foreach (PnLine *line, lineIMap) {
    uint32_t NoIid = line->noI()->id;
    uint32_t NoFid = line->noF()->id;

    if ((NoIid == noI) || (NoIid == noF))
      if ((NoFid == noI) || (NoFid == noF)) return line;
  }
  return NULL;
}

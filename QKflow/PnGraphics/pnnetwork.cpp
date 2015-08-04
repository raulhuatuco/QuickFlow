#include "pnnetwork.h"

PnNetwork::PnNetwork() {}

PnNetwork::~PnNetwork() {}

bool PnNetwork::addSlack(PnSlack *slack) {
  addItem(slack);
  barMap.insert(0, slack);
  return true;
}

bool PnNetwork::addPq(PnPq *pq) {
  addItem(pq);
  barMap.insert(pq->Id(), pq);
  return true;
}

bool PnNetwork::addPv(PnPv *pv) {
  addItem(pv);
  barMap.insert(pv->Id(), pv);
  return true;
}

bool PnNetwork::addCable(PnCable *cable) {
  addItem(cable);
  lineMap.insert(cable->Id(), cable);
  return true;
}

PnBar *PnNetwork::getBarById(unsigned int id) { return barMap.value(id, NULL); }

PnLine *PnNetwork::getLineById(unsigned int id) {
  return lineMap.value(id, NULL);
}

PnLine *PnNetwork::getLineByNodes(unsigned int noI, unsigned int noF) {
  foreach (PnLine *line, lineMap) {
    unsigned int NoIid = line->NoI()->Id();
    unsigned int NoFid = line->NoF()->Id();
    if ((NoIid == noI) || (NoIid == noF))
      if ((NoFid == noI) || (NoFid == noF)) return line;
  }
  return NULL;
}

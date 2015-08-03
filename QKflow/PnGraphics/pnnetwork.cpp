#include "pnnetwork.h"

PnNetwork::PnNetwork() {

}

PnNetwork::~PnNetwork() {

}

bool PnNetwork::addSlack(PnSlack *slack) {

  if (getBarById(0) != NULL)
    return false;

  addItem(slack);
  pnHash_.insert(0, slack);
  return true;
}

bool PnNetwork::addPq(PnPq *pq) {

  if (getBarById(pq->getId()) != NULL)
    return true;

  addItem(pq);
  pnHash_.insert(pq->getId(), pq);
  return true;
}

bool PnNetwork::addPv(PnPv *pv) {
  if (getBarById(pv->getId()) != NULL)
    return true;

  addItem(pv);
  pnHash_.insert(pv->getId(), pv);
  return true;
}

PnBar *PnNetwork::getBarById(unsigned int id) {
  return pnHash_.value(id, NULL);
}


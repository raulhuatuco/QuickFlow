#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QMap>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"

class PnNetwork : public QGraphicsScene {
 public:
  PnNetwork();
  ~PnNetwork();

  QMap<uint32_t, PnBar *> barMap;
  QMap<uint32_t, PnLine *> lineMap;

  bool addSlack(PnSlack *slack);
  bool addPq(PnPq *pq);
  bool addPv(PnPv *pv);

  bool addCable(PnCable *cable);

  PnBar *getBarById(unsigned int id);
  PnLine *getLineById(unsigned int id);
  PnLine *getLineByNodes(unsigned int noI, unsigned int noF);

 private:
};

#endif  // PNNETWORK_H

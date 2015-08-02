#ifndef PNNETWORK_H
#define PNNETWORK_H

#include <QGraphicsScene>
#include <QHash>

#include "PnGraphics/pnbar.h"
#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"

class PnNetwork : public QGraphicsScene
{
public:
    PnNetwork();
    ~PnNetwork();

    bool addSlack(PnSlack *slack);
    bool addPq(PnPq *pq);
    bool addPv(PnPv *pv);

    PnBar *getBarById(unsigned int id);

private:
    QHash<uint32_t, PnBar *> pnHash_;
};

#endif // PNNETWORK_H

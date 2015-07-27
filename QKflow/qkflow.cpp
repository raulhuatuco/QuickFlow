#include "qkflow.h"
#include "ui_qkflow.h"

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"

QKflow::QKflow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QKflow)
{
    ui->setupUi(this);

    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);


    PnSlack::ico = new QPixmap(":/img/ico-slack.png");
    PnSlack::icoSelected = new QPixmap(":/img/ico-slack-s.png");
    PnSlack *slack = new PnSlack();
    ui->pnView->scene ()->addItem (slack);

    PnPq::ico = new QPixmap(":/img/ico-pq.png");
    PnPq::icoSelected = new QPixmap(":/img/ico-pq-s.png");
    PnPq *pq = new PnPq();
    pq->setX (100);
    ui->pnView->scene ()->addItem (pq);

    PnPv::ico = new QPixmap(":/img/ico-pv.png");
    PnPv::icoSelected = new QPixmap(":/img/ico-pv-s.png");
    PnPv *pv = new PnPv();
    pv->setX (200);
    ui->pnView->scene ()->addItem (pv);

    PnCable *cable1 = new PnCable();
    slack->addLine (cable1);
    pq->addLine (cable1);
    ui->pnView->scene ()->addItem (cable1);
    cable1->setNodes (slack, pq);
}

QKflow::~QKflow()
{
    delete ui;
}

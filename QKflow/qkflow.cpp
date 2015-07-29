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
    PnPq *pq = new PnPq(1);
    pq->setX(100);
    pq->setY(100);
    ui->pnView->scene ()->addItem (pq);

    PnPv::ico = new QPixmap(":/img/ico-pv.png");
    PnPv::icoSelected = new QPixmap(":/img/ico-pv-s.png");
    PnPv *pv = new PnPv(2);
    pv->setX (200);
    pv->setY(-50);
    ui->pnView->scene ()->addItem (pv);

    PnCable *cable1 = new PnCable(slack, pq);
    ui->pnView->scene ()->addItem (cable1);

    PnCable *cable2 = new PnCable(slack, pv);
    ui->pnView->scene ()->addItem (cable2);

    PnCable *cable3 = new PnCable(pq, pv);
    ui->pnView->scene ()->addItem (cable3);


}

QKflow::~QKflow()
{
    delete ui;
}

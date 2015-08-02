#include "qkflow.h"
#include "ui_qkflow.h"

#include <QMessageBox>

#include "PnGraphics/pnslack.h"
#include "PnGraphics/pnpq.h"
#include "PnGraphics/pnpv.h"
#include "PnGraphics/pncable.h"
#include "PnGraphics/pnnetwork.h"

QKflow::QKflow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QKflow)
{
    ui->setupUi(this);

    ui->actionPause->setDisabled(true);
    ui->actionStop->setDisabled(true);


    // Create network
    pnNetwork_ = new PnNetwork;
    ui->pnView->setPnNetwork(pnNetwork_);

    // Set background color
    QBrush bkbrush;
    bkbrush.setColor(QColor::fromRgbF(0.0,1.0,0.0, 0.15));
    bkbrush.setStyle(Qt::SolidPattern);
    ui->pnView->setBackgroundBrush(bkbrush);


    // Add some items
    PnSlack *slack = new PnSlack();
    pnNetwork_->addSlack(slack);

    PnPq *pq = new PnPq(1);
    pq->setPos(100,100);
    pnNetwork_->addPq(pq);

    PnPv *pv = new PnPv(2);
    pv->setX (200);
    pv->setY(-50);
    pnNetwork_->addPv(pv);

    PnCable *cable1 = new PnCable(slack, pq);
    pnNetwork_->addItem (cable1);

    PnCable *cable2 = new PnCable(slack, pv);
    pnNetwork_->addItem (cable2);

    PnCable *cable3 = new PnCable(pq, pv);
    pnNetwork_->addItem (cable3);


}

QKflow::~QKflow()
{
    delete ui;
}

void QKflow::on_actionZoomIn_triggered()
{
    ui->pnView->zoomIn();
}

void QKflow::on_actionZoomOut_triggered()
{
    ui->pnView->zoomOut();
}

void QKflow::on_actionZoomFit_triggered()
{
    ui->pnView->zoomFit();
}

void QKflow::on_actionSlack_triggered()
{
    PnSlack *slack = new PnSlack();
    windowAddSlack = new WindowAddSlack(this, slack);

    if(windowAddSlack->exec() == QDialog::Accepted) {
        if(pnNetwork_->addSlack(slack) == false){
            delete slack;
            QMessageBox::critical(this, tr("Failed to add Slack Bar"), tr("Only 1 slack bar is allowed."), QMessageBox::Ok);
        }
    }else{
        delete slack;
    }

    delete windowAddSlack;
}

void QKflow::on_actionPQBar_triggered()
{
    PnPq *pq = new PnPq();
    windowAddPq = new WindowAddPq(this, pq);

    if(windowAddPq->exec() == QDialog::Accepted) {
        if(pnNetwork_->addPq(pq) == false){
            delete pq;
            QMessageBox::critical(this, tr("Failed to add PQ Bar"), tr("Bars must have an unique identifier."), QMessageBox::Ok);
        }
    }else{
        delete pq;
    }

    delete windowAddPq;
}

#include "windowaddpq.h"
#include "ui_windowaddpq.h"

#include <QMessageBox>

WindowAddPq::WindowAddPq(QWidget *parent, PnPq *pq) :
    QDialog(parent),
    ui(new Ui::WindowAddPq), pq_(pq)
{
    ui->setupUi(this);
}

WindowAddPq::~WindowAddPq()
{
    delete ui;
}

void WindowAddPq::on_btnOk_clicked()
{
    if (pq_ == NULL) {
        QMessageBox::critical(this, tr("Cannot create PQ Bar"), tr("Failled to alloc PQ bar."), QMessageBox::Ok);
        reject();
    }

    pq_->setId(ui->spbId->value());

    if (ui->ledPx->text().isEmpty() || ui->ledPy->text().isEmpty()){
        QMessageBox::information(this, tr("Invalid position"), tr("Invalid bar position!"), QMessageBox::Ok);
        return;
    }

    pq_->setPos(ui->ledPx->text().toDouble(), ui->ledPy->text().toDouble());

    accept();
}

void WindowAddPq::on_btnCancel_clicked()
{
    reject();
}

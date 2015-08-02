#include "windowaddslack.h"
#include "ui_windowaddslack.h"

WindowAddSlack::WindowAddSlack(QWidget *parent, PnSlack *slack) :
    QDialog(parent),
    ui(new Ui::WindowAddSlack), slack_(slack)
{
    ui->setupUi(this);

}

WindowAddSlack::~WindowAddSlack()
{

    delete ui;
}

void WindowAddSlack::on_btnCancel_clicked()
{
    reject();
}

void WindowAddSlack::on_btnOk_clicked()
{
    accept();
}

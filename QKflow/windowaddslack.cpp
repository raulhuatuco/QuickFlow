#include "windowaddslack.h"
#include "ui_windowaddslack.h"

#include <QValidator>
#include <QMessageBox>
#include <complex>

WindowAddSlack::WindowAddSlack(QWidget *parent,
                               PnSlack *slack) :
  QDialog(parent),
  ui(new Ui::WindowAddSlack), slack_(slack) {
  ui->setupUi(this);

  ui->ledPload->setValidator (new QDoubleValidator(0, qInf(), 1000, this));
  ui->ledQload->setValidator (new QDoubleValidator(0, qInf(), 1000, this));
  ui->ledMaxCapacity->setValidator (new QDoubleValidator(0, qInf(), 1000, this));
  ui->ledPosX->setValidator (new QDoubleValidator(0, qInf(), 1000, this));
  ui->ledPosY->setValidator (new QDoubleValidator(0, qInf(), 1000, this));
}

WindowAddSlack::~WindowAddSlack() {
  delete ui;
}

void WindowAddSlack::on_btnCancel_clicked() {
  reject();
}

void WindowAddSlack::on_btnOk_clicked() {
  if (ui->ledPload->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Load Power."), QMessageBox::Ok);
    ui->ledPload->setFocus();
    return;
  }

  if (ui->ledQload->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Load Reactive Power."), QMessageBox::Ok);
    ui->ledQload->setFocus();
    return;
  }

  if (ui->ledMaxCapacity->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Max Capacity."), QMessageBox::Ok);
    ui->ledMaxCapacity->setFocus();
    return;
  }

  if (ui->ledPosX->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("X Coordenate is empty."), QMessageBox::Ok);
    ui->ledPosX->setFocus();
    return;
  }

  if (ui->ledPosY->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Y Coordenate is empty."), QMessageBox::Ok);
    ui->ledPosY->setFocus();
    return;
  }

  std::complex<double> sl;
  sl.real(ui->ledPload->text().toDouble());
  sl.imag(ui->ledQload->text().toDouble());
  slack_->setSl(sl);

  slack_->setMaxGeneration(
    ui->ledMaxCapacity->text().toDouble());

  slack_->setPos(ui->ledPosX->text().toDouble(),
                 ui->ledPosY->text().toDouble());
  accept();
}

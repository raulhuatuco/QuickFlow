#include "windowaddpq.h"
#include "ui_windowaddpq.h"

#include <QValidator>
#include <QMessageBox>

WindowAddPq::WindowAddPq(QWidget *parent, PnPq *pq) :
  QDialog(parent),
  ui(new Ui::WindowAddPq), pq_(pq) {
  ui->setupUi(this);

  ui->Pgen->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qgen->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Pload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Px->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Py->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
}

WindowAddPq::~WindowAddPq() {
  delete ui;
}

void WindowAddPq::on_btnOk_clicked() {

  if (ui->Pgen->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Generated Power."), QMessageBox::Ok);
    ui->Pgen->setFocus();
    return;
  }

  if (ui->Qgen->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Generated Reactive Power."), QMessageBox::Ok);
    ui->Qgen->setFocus();
    return;
  }

  if (ui->Pload->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Load Power."), QMessageBox::Ok);
    ui->Pload->setFocus();
    return;
  }

  if (ui->Qload->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Load Reactive Power."), QMessageBox::Ok);
    ui->Qload->setFocus();
    return;
  }

  if (ui->Px->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty X position."), QMessageBox::Ok);
    ui->Px->setFocus();
    return;
  }

  if (ui->Py->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Y position"), QMessageBox::Ok);
    ui->Py->setFocus();
    return;
  }

  pq_->setId(ui->id->value());

  std::complex<double> sg;
  sg.real(ui->Pgen->text().toDouble());
  sg.imag(ui->Qgen->text().toDouble());
  pq_->setSl(sg);


  std::complex<double> sl;
  sl.real(ui->Pload->text().toDouble());
  sl.imag(ui->Qload->text().toDouble());
  pq_->setSl(sl);

  pq_->setPos(ui->Px->text().toDouble(), ui->Py->text().toDouble());
  accept();
}

void WindowAddPq::on_btnCancel_clicked() {
  reject();
}

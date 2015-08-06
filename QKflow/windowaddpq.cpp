#include "windowaddpq.h"
#include "ui_windowaddpq.h"

#include <QValidator>
#include <QMessageBox>

#include "PnGraphics/pnpq.h"

WindowAddPq::WindowAddPq(QWidget *parent)
  : QDialog(parent), ui(new Ui::WindowAddPq), pnNetwork_(NULL) {
  ui->setupUi(this);

  ui->Pgen->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qgen->setValidator(new QDoubleValidator(this));
  ui->Pload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qload->setValidator(new QDoubleValidator(this));
  ui->Px->setValidator(new QDoubleValidator(this));
  ui->Py->setValidator(new QDoubleValidator(this));

  ui->GenerationUnity->addItem("VA", 1.0);
  ui->GenerationUnity->addItem("KVA", 1.0e3);
  ui->GenerationUnity->addItem("MVA", 1.0e6);
  ui->GenerationUnity->addItem("GVA", 1.0e9);

  ui->LoadUnity->addItem("VA", 1.0);
  ui->LoadUnity->addItem("KVA", 1.0e3);
  ui->LoadUnity->addItem("MVA", 1.0e6);
}

WindowAddPq::~WindowAddPq() {
  delete ui;
}

void WindowAddPq::setNetwork(PnNetwork *pnNetwork) {
  pnNetwork_ = pnNetwork;
}

void WindowAddPq::on_btnOk_clicked() {
  if (pnNetwork_ == NULL) {
    QMessageBox::critical(this, tr("Invalid Network"),
                          tr("Invalid Network parameter!"), QMessageBox::Ok);
    reject();
  }

  if (pnNetwork_->getBarById(ui->id->value()) != NULL) {
    QMessageBox::information(this, tr("Id already exist"),
                             tr("Another bar is using this ID."),
                             QMessageBox::Ok);
    ui->id->setFocus();
    return;
  }

  if (ui->Pgen->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Generated Power."), QMessageBox::Ok);
    ui->Pgen->setFocus();
    return;
  }

  if (ui->Qgen->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Generated Reactive Power."),
                             QMessageBox::Ok);
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

  PnPq *pq = new PnPq;
  pq->setId(ui->id->value());

  std::complex<double> sg;
  sg.real(ui->Pgen->text().toDouble());
  sg.imag(ui->Qgen->text().toDouble());
  double sgUnity = ui->GenerationUnity->currentData().toDouble();
  sg *= sgUnity;
  pq->setInputSl(sg);

  std::complex<double> sl;
  sl.real(ui->Pload->text().toDouble());
  sl.imag(ui->Qload->text().toDouble());
  double slUnity = ui->LoadUnity->currentData().toDouble();
  sl *= slUnity;
  pq->setInputSl(sl);

  pq->setPos(ui->Px->text().toDouble(), ui->Py->text().toDouble());

  pnNetwork_->addPq(pq);

  accept();
}

void WindowAddPq::on_btnCancel_clicked() {
  reject();
}

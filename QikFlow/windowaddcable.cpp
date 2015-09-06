#include "windowaddcable.h"
#include "ui_windowaddcable.h"

#include <QValidator>
#include <QMessageBox>

#include "PnGraphics/pncable.h"

WindowAddCable::WindowAddCable(QWidget *parent)
  : QDialog(parent), ui(new Ui::WindowAddCable), pnNetwork_(NULL) {
  ui->setupUi(this);

  ui->R->setValidator(new QDoubleValidator(this));
  ui->X->setValidator(new QDoubleValidator(this));
  ui->Pmax->setValidator(new QDoubleValidator(this));
  ui->Qmax->setValidator(new QDoubleValidator(this));

  ui->LoadUnity->addItem("VA", 1.0);
  ui->LoadUnity->addItem("KVA", 1.0e3);
  ui->LoadUnity->addItem("MVA", 1.0e6);
  ui->LoadUnity->addItem("GVA", 1.0e9);

  ui->ImpedanceUnity->addItem("Ohm");
  ui->ImpedanceUnity->addItem("Ohm/m");
  ui->ImpedanceUnity->addItem("Ohm/km");
  ui->ImpedanceUnity->addItem("Ohm/in");
  ui->ImpedanceUnity->addItem("Ohm/ft");
  ui->ImpedanceUnity->addItem("Ohm/mile");

  ui->LengthUnit->addItem("m", 1.0);
  ui->LengthUnit->addItem("km", 1.0e3);
  ui->LengthUnit->addItem("in", 39.3701);
  ui->LengthUnit->addItem("ft", 3.28084);
  ui->LengthUnit->addItem("mile", 0.000621371);
}

WindowAddCable::~WindowAddCable() {
  delete ui;
}

void WindowAddCable::setNetwork(PnNetwork *pnNetwork) {
  pnNetwork_ = pnNetwork;

  foreach (PnBar *bar, pnNetwork->barMap) {
    ui->NoI->addItem(QString::number(bar->Id()));
    ui->NoF->addItem(QString::number(bar->Id()));
  }
}

void WindowAddCable::on_btnCancel_clicked() {
  reject();
}

void WindowAddCable::on_btnOk_clicked() {
  if (pnNetwork_ == NULL) {
    QMessageBox::critical(this, tr("Invalid Network"),
                          tr("Invalid Network parameter!"), QMessageBox::Ok);
    reject();
  }

  if (pnNetwork_->getLineById(ui->id->value()) != NULL) {
    QMessageBox::information(this, tr("Id already exist"),
                             tr("Another line is using this ID."),
                             QMessageBox::Ok);
    ui->id->setFocus();
    return;
  }

  if (ui->NoI->currentText().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty initial node."), QMessageBox::Ok);
    ui->NoI->setFocus();
    return;
  }

  if (ui->NoF->currentText().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty final node."), QMessageBox::Ok);
    ui->NoF->setFocus();
    return;
  }

  if (ui->NoI->currentText() == ui->NoF->currentText()) {
    QMessageBox::information(
      this, tr("Invalid Parameter"),
      tr("Initial node must be diferent from final node."), QMessageBox::Ok);
    ui->NoI->setFocus();
    return;
  }

  if (pnNetwork_->getLineByNodes(ui->NoI->currentText().toUInt(),
                                 ui->NoF->currentText().toUInt()) != NULL) {
    QMessageBox::information(this, tr("Line already exist"),
                             tr("Another line is connecting the same bars"),
                             QMessageBox::Ok);
    ui->NoI->setFocus();
    return;
  }

  if (ui->R->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Resistence."), QMessageBox::Ok);
    ui->R->setFocus();
    return;
  }

  if (ui->X->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Reactance."), QMessageBox::Ok);
    ui->X->setFocus();
    return;
  }

  if (ui->Pmax->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty maximum load."), QMessageBox::Ok);
    ui->Pmax->setFocus();
    return;
  }

  if (ui->Qmax->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty maximum reactive load."),
                             QMessageBox::Ok);
    ui->Qmax->setFocus();
    return;
  }

  PnCable *cable = new PnCable;
  cable->setId(ui->id->value());

  PnBar *nodeInitial = pnNetwork_->getBarById(ui->NoI->currentText().toUInt());
  PnBar *nodeFinal = pnNetwork_->getBarById(ui->NoF->currentText().toUInt());

  cable->setNodes(nodeInitial, nodeFinal);

  std::complex<double> z;
  z.real(ui->R->text().toDouble());
  z.imag(ui->X->text().toDouble());

  if (ui->ImpedanceUnity->currentIndex() != 0) {
    double length;
    length = ui->Length->text().toDouble();
    length *= ui->LengthUnit->currentData().toDouble();
  }

  cable->setImpedance(z);

  pnNetwork_->addCable(cable);

  accept();
}

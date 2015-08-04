#include "windowaddslack.h"
#include "ui_windowaddslack.h"

#include <QValidator>
#include <QMessageBox>
#include <complex>
#include <math.h>

#include "PnGraphics/pnslack.h"

WindowAddSlack::WindowAddSlack(QWidget *parent)
    : QDialog(parent), ui(new Ui::WindowAddSlack), pnNetwork_(NULL) {
  ui->setupUi(this);

  ui->MaxCapacity->setValidator(new QDoubleValidator(0, qInf(), 1000, this));

  ui->Pload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qload->setValidator(new QDoubleValidator(this));
  ui->V->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->phase->setValidator(new QDoubleValidator(this));
  ui->Px->setValidator(new QDoubleValidator(this));
  ui->Py->setValidator(new QDoubleValidator(this));

  ui->VUnity->addItem("V", 1.0);
  ui->VUnity->addItem("KV", 1.0e3);
  ui->VUnity->addItem("MV", 1.0e6);
  ui->VUnity->addItem("GV", 1.0e9);

  ui->GenerationUnity->addItem("W", 1.0);
  ui->GenerationUnity->addItem("KW", 1.0e3);
  ui->GenerationUnity->addItem("MW", 1.0e6);
  ui->GenerationUnity->addItem("GW", 1.0e9);

  ui->LoadUnity->addItem("VA", 1.0);
  ui->LoadUnity->addItem("KVA", 1.0e3);
  ui->LoadUnity->addItem("MVA", 1.0e6);
  ui->LoadUnity->addItem("GVA", 1.0e9);
}

WindowAddSlack::~WindowAddSlack() { delete ui; }

void WindowAddSlack::setNetwork(PnNetwork *pnNetwork) {
  pnNetwork_ = pnNetwork;
}

void WindowAddSlack::on_btnCancel_clicked() { reject(); }

void WindowAddSlack::on_btnOk_clicked() {
  if (pnNetwork_ == NULL) {
    QMessageBox::critical(this, tr("Invalid Network"),
                          tr("Invalid Network parameter!"), QMessageBox::Ok);
    reject();
  }

  if (pnNetwork_->getBarById(0) != NULL) {
    QMessageBox::critical(this, tr("Multiple Slack Bars"),
                          tr("Only one slack bar allowed!"), QMessageBox::Ok);
    reject();
  }

  if (ui->V->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty voltage."), QMessageBox::Ok);
    ui->V->setFocus();
    return;
  }

  if (ui->phase->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"), tr("Empty phase."),
                             QMessageBox::Ok);
    ui->phase->setFocus();
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

  if (ui->MaxCapacity->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Max Capacity."), QMessageBox::Ok);
    ui->MaxCapacity->setFocus();
    return;
  }

  if (ui->Px->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("X Coordenate is empty."), QMessageBox::Ok);
    ui->Px->setFocus();
    return;
  }

  if (ui->Py->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Y Coordenate is empty."), QMessageBox::Ok);
    ui->Py->setFocus();
    return;
  }

  PnSlack *slack = new PnSlack;

  double vUnity = ui->VUnity->currentData().toDouble();
  double voltage = ui->V->text().toDouble() * vUnity;
  double phase = ui->phase->text().toDouble() * 180 / M_PI;
  std::complex<double> v;
  v.real(voltage * cos(phase));
  v.imag(voltage * sin(phase));
  slack->setInputV(v);

  double sgUnity = ui->GenerationUnity->currentData().toDouble();
  slack->setMaxGeneration(ui->MaxCapacity->text().toDouble() * sgUnity);

  std::complex<double> sl;
  sl.real(ui->Pload->text().toDouble());
  sl.imag(ui->Qload->text().toDouble());
  double slUnity = ui->LoadUnity->currentData().toDouble();
  sl *= slUnity;
  slack->setInputSl(sl);

  slack->setPos(ui->Px->text().toDouble(), ui->Py->text().toDouble());

  pnNetwork_->addSlack(slack);
  accept();
}

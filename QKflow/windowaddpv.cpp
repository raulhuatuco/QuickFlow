#include "windowaddpv.h"
#include "ui_windowaddpv.h"

#include <QValidator>
#include <QMessageBox>

#include "PnGraphics/pnpv.h"

WindowAddPv::WindowAddPv(QWidget *parent)
    : QDialog(parent), ui(new Ui::WindowAddPv), pnNetwork_(NULL) {
  ui->setupUi(this);

  ui->V->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Pgen->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->QgenMax->setValidator(new QDoubleValidator(this));
  ui->QgenMin->setValidator(new QDoubleValidator(this));
  ui->Pload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->Qload->setValidator(new QDoubleValidator(this));
  ui->Px->setValidator(new QDoubleValidator(this));
  ui->Py->setValidator(new QDoubleValidator(this));

  ui->VUnity->addItem("V", 1.0);
  ui->VUnity->addItem("KV", 1.0e3);
  ui->VUnity->addItem("MV", 1.0e6);

  ui->GenerationUnity->addItem("VA", 1.0);
  ui->GenerationUnity->addItem("KVA", 1.0e3);
  ui->GenerationUnity->addItem("MVA", 1.0e6);
  ui->GenerationUnity->addItem("GVA", 1.0e9);

  ui->LoadUnity->addItem("VA", 1.0);
  ui->LoadUnity->addItem("KVA", 1.0e3);
  ui->LoadUnity->addItem("MVA", 1.0e6);
  ui->LoadUnity->addItem("GVA", 1.0e9);
}

WindowAddPv::~WindowAddPv() { delete ui; }

void WindowAddPv::setNetwork(PnNetwork *pnNetwork) { pnNetwork_ = pnNetwork; }

void WindowAddPv::on_btnCancel_clicked() { reject(); }

void WindowAddPv::on_btnOk_clicked() {
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

  if (ui->V->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty voltage."), QMessageBox::Ok);
    ui->V->setFocus();
    return;
  }

  if (ui->Pgen->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Generated Power."), QMessageBox::Ok);
    ui->Pgen->setFocus();
    return;
  }

  if (ui->QgenMax->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Maximum Generated Reactive Power."),
                             QMessageBox::Ok);
    ui->QgenMax->setFocus();
    return;
  }

  if (ui->QgenMin->text().isEmpty()) {
    QMessageBox::information(this, tr("Missing Parameter"),
                             tr("Empty Minimum Generated Reactive Power."),
                             QMessageBox::Ok);
    ui->QgenMin->setFocus();
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

  PnPv *pv = new PnPv;
  pv->setId(ui->id->value());

  double V = ui->V->text().toDouble() * ui->VUnity->currentData().toDouble();
  pv->setInputV(std::polar(V, 0.0));

  std::complex<double> sg;
  sg.real(ui->Pgen->text().toDouble());
  sg.imag(0.0);
  double sgUnity = ui->GenerationUnity->currentData().toDouble();
  sg *= sgUnity;
  pv->setInputSl(sg);
  double maxQ = ui->QgenMax->text().toDouble();
  pv->setMaxQGenerated(maxQ * sgUnity);
  double minQ = ui->QgenMin->text().toDouble();
  pv->setMinQGenerated(minQ * sgUnity);

  std::complex<double> sl;
  sl.real(ui->Pload->text().toDouble());
  sl.imag(ui->Qload->text().toDouble());
  double slUnity = ui->LoadUnity->currentData().toDouble();
  sl *= slUnity;
  pv->setInputSl(sl);

  pv->setPos(ui->Px->text().toDouble(), ui->Py->text().toDouble());

  pnNetwork_->addPv(pv);

  accept();
}

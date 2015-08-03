#include "windowaddpv.h"

#include <QLabel>

WindowAddPv::WindowAddPv(QWidget *parent) {

  Pgen = new QLineEdit(this);
  Pload = new QLineEdit(this);
  Qload = new QLineEdit(this);
  MaxQGeneration = new QLineEdit(this);
  Px = new QLineEdit(this);
  Py = new QLineEdit(this);

  Pgen->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  Pload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  Qload->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  MaxQGeneration->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  Px->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  Py->setValidator(new QDoubleValidator(0, qInf(), 1000, this));


}

WindowAddPv::~WindowAddPv() {

}

void WindowAddPv::setNetwork(PnNetwork *network) {
  network_ = network;
}

void WindowAddPv::on_btnOk_clicked() {

}

void WindowAddPv::on_btnCancel_clicked() {

}


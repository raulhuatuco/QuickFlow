#include "barproperties.h"
#include "ui_barproperties.h"
#include <QValidator>
#include <QMessageBox>

/*******************************************************************************
 * WindowBarProperties.
 ******************************************************************************/
BarProperties::BarProperties(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::BarProperties)
{
  ui->setupUi(this);

  // Create input validators.
  ui->Va->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Vb->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Vc->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Vai->setValidator(new QDoubleValidator(this));
  ui->Vbi->setValidator(new QDoubleValidator(this));
  ui->Vci->setValidator(new QDoubleValidator(this));

  ui->Sha->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Shb->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Shc->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Shai->setValidator(new QDoubleValidator(this));
  ui->Shbi->setValidator(new QDoubleValidator(this));
  ui->Shci->setValidator(new QDoubleValidator(this));

  ui->Sia->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Sib->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Sic->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Siai->setValidator(new QDoubleValidator(this));
  ui->Sibi->setValidator(new QDoubleValidator(this));
  ui->Sici->setValidator(new QDoubleValidator(this));

  ui->px->setValidator(new QDoubleValidator(this));
  ui->py->setValidator(new QDoubleValidator(this));
}

/*******************************************************************************
 * ~WindowBarProperties.
 ******************************************************************************/
BarProperties::~BarProperties()
{
  delete ui;
}

/*******************************************************************************
 * setOptions.
 ******************************************************************************/
void BarProperties::setOptions(Project *project, Bar *bar)
{
  // Adjust apearance according to bar.
  if (bar == NULL) {
    setWindowTitle(tr("New Bar"));
    // Create a new bar.
    ui->id->setValue(0);
    bar_ = new Bar;
    isNew = true;
    foreach(Network *network, project->networks) {
      ui->network->addItem(network->name);
    }
  } else {
    setWindowTitle(tr("Edit Bar ") + QString::number(bar->id));
    ui->id->setEnabled(false);
    // Store parameters.
    bar_ = bar;
    ui->id->setValue(bar_->id);
    isNew = false;
    ui->network->addItem(bar->network->name);
    ui->network->setEnabled(false);
  }

// Store project parameters.
  project_ = project;

// Fill Bar data.
  ui->Va->setText(QString::number(bar_->v(0,Network::voltageUnit).real()));
  ui->Vb->setText(QString::number(bar_->v(1,Network::voltageUnit).real()));
  ui->Vc->setText(QString::number(bar_->v(2,Network::voltageUnit).real()));
  ui->Vai->setText(QString::number(bar_->v(0,Network::voltageUnit).imag()));
  ui->Vbi->setText(QString::number(bar_->v(1,Network::voltageUnit).imag()));
  ui->Vci->setText(QString::number(bar_->v(2,Network::voltageUnit).imag()));
  ui->Sha->setText(QString::number(bar_->sh(0, Network::powerUnit).real()));
  ui->Shb->setText(QString::number(bar_->sh(1, Network::powerUnit).real()));
  ui->Shc->setText(QString::number(bar_->sh(2, Network::powerUnit).real()));
  ui->Shai->setText(QString::number(bar_->sh(0, Network::powerUnit).imag()));
  ui->Shbi->setText(QString::number(bar_->sh(1, Network::powerUnit).imag()));
  ui->Shci->setText(QString::number(bar_->sh(2, Network::powerUnit).imag()));
  ui->Sia->setText(QString::number(bar_->si(0, Network::powerUnit).real()));
  ui->Sib->setText(QString::number(bar_->si(1, Network::powerUnit).real()));
  ui->Sic->setText(QString::number(bar_->si(2, Network::powerUnit).real()));
  ui->Siai->setText(QString::number(bar_->si(0, Network::powerUnit).imag()));
  ui->Sibi->setText(QString::number(bar_->si(1, Network::powerUnit).imag()));
  ui->Sici->setText(QString::number(bar_->si(2, Network::powerUnit).imag()));
  ui->px->setText(QString::number(bar_->x()));
  ui->py->setText(QString::number(bar_->y()));

  // Set units.
  // Voltage.
  ui->VaUn->setText(tr("[") + Unit::voltageUnitToStr(Network::voltageUnit) +
                    tr("]"));
  ui->VbUn->setText(tr("[") + Unit::voltageUnitToStr(Network::voltageUnit) +
                    tr("]"));
  ui->VcUn->setText(tr("[") + Unit::voltageUnitToStr(Network::voltageUnit) +
                    tr("]"));

  // Power.
  ui->ShaUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
  ui->SiaUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
  ui->ShbUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
  ui->SibUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
  ui->ShcUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
  ui->SicUn->setText(tr("[") + Unit::powerUnitToStr(Network::powerUnit) +
                     tr("]"));
}

Bar *BarProperties::bar()
{
  return bar_;
}

/*******************************************************************************
 * Botton Box Accepted.
 ******************************************************************************/
void BarProperties::on_buttonBox_accepted()
{

  Network *network = project_->networks.value(ui->network->currentText(), NULL);

  if (network == NULL) {
    QMessageBox::information(this, "Invalid parameter", "Invalid network.",
                             QMessageBox::Ok);
    ui->network->setFocus();
    return;
  }

  // Check for valid id.
  if(isNew) {
    if (network->getBarById(ui->id->value()) != NULL) {
      QMessageBox::information(this, "Invalid parameter", "Id already in use.",
                               QMessageBox::Ok);
      ui->id->setFocus();
      return;
    }
  }

//  // Check for empty fields.
//  // Voltages.
//  // Real Part.
  if (ui->Va->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Va is empty.",
                             QMessageBox::Ok);
    ui->Va->setFocus();
    return;
  }

  if (ui->Vb->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Vb is empty.",
                             QMessageBox::Ok);
    ui->Vb->setFocus();
    return;
  }

  if (ui->Vc->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Vc is empty.",
                             QMessageBox::Ok);
    ui->Vc->setFocus();
    return;
  }

  // Imaginary Part.
  if (ui->Vai->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Va is empty.",
                             QMessageBox::Ok);
    ui->Vai->setFocus();
    return;
  }

  if (ui->Vbi->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Vb is empty.",
                             QMessageBox::Ok);
    ui->Vbi->setFocus();
    return;
  }

  if (ui->Vci->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Vc is empty.",
                             QMessageBox::Ok);
    ui->Vci->setFocus();
    return;
  }

  // Generated Power.
  // Real Part.
  if (ui->Sha->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sha is empty.",
                             QMessageBox::Ok);
    ui->Sha->setFocus();
    return;
  }

  if (ui->Shb->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Shb is empty.",
                             QMessageBox::Ok);
    ui->Shb->setFocus();
    return;
  }

  if (ui->Shc->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Shc is empty.",
                             QMessageBox::Ok);
    ui->Shc->setFocus();
    return;
  }

  // Imaginary Part.
  if (ui->Shai->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sha is empty.",
                             QMessageBox::Ok);
    ui->Shai->setFocus();
    return;
  }

  if (ui->Shbi->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Shb is empty.",
                             QMessageBox::Ok);
    ui->Shbi->setFocus();
    return;
  }

  if (ui->Shci->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Shc is empty.",
                             QMessageBox::Ok);
    ui->Shci->setFocus();
    return;
  }

  // Consumed Power.
  // Real Part.
  if (ui->Sia->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sia is empty.",
                             QMessageBox::Ok);
    ui->Sia->setFocus();
    return;
  }

  if (ui->Sib->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sib is empty.",
                             QMessageBox::Ok);
    ui->Sib->setFocus();
    return;
  }

  if (ui->Sic->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sic is empty.",
                             QMessageBox::Ok);
    ui->Sic->setFocus();
    return;
  }

  // Imaginary Part.
  if (ui->Siai->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sia is empty.",
                             QMessageBox::Ok);
    ui->Siai->setFocus();
    return;
  }

  if (ui->Sibi->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sib is empty.",
                             QMessageBox::Ok);
    ui->Sibi->setFocus();
    return;
  }

  if (ui->Sici->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sic is empty.",
                             QMessageBox::Ok);
    ui->Sici->setFocus();
    return;
  }

  // Position
  if (ui->px->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter X is empty.",
                             QMessageBox::Ok);
    ui->px->setFocus();
    return;
  }

  if (ui->py->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Y is empty.",
                             QMessageBox::Ok);
    ui->py->setFocus();
    return;
  }

  // Replace Bar Data.
  // Id.
  bar_->id = ui->id->value();

  // Network
  bar_->network = network;

  // Voltage.
  complex<double> voltage;
  voltage.real(ui->Va->text().toDouble());
  voltage.imag(ui->Vai->text().toDouble());
  bar_->setV(0,voltage, Network::voltageUnit);

  voltage.real(ui->Vb->text().toDouble());
  voltage.imag(ui->Vbi->text().toDouble());
  bar_->setV(1, voltage, Network::voltageUnit);


  voltage.real(ui->Vc->text().toDouble());
  voltage.imag(ui->Vci->text().toDouble());
  bar_->setV(2, voltage, Network::voltageUnit);

  // Generated power.
  complex<double> shunt;

  shunt.real(ui->Sha->text().toDouble());
  shunt.imag(ui->Shai->text().toDouble());
  bar_->setSh(0, shunt, Network::powerUnit);

  shunt.real(ui->Shb->text().toDouble());
  shunt.imag(ui->Shbi->text().toDouble());
  bar_->setSh(1, shunt, Network::powerUnit);

  shunt.real(ui->Shc->text().toDouble());
  shunt.imag(ui->Shci->text().toDouble());
  bar_->setSh(2, shunt, Network::powerUnit);

  // Consumed power.
  complex<double> injected;
  injected.real(ui->Sia->text().toDouble());
  injected.imag(ui->Siai->text().toDouble());
  bar_->setSi(0, injected, Network::powerUnit);

  injected.real(ui->Sib->text().toDouble());
  injected.imag(ui->Sibi->text().toDouble());
  bar_->setSi(1, injected, Network::powerUnit);

  injected.real(ui->Sic->text().toDouble());
  injected.imag(ui->Sici->text().toDouble());
  bar_->setSi(2, injected, Network::powerUnit);

  // Position
  if((ui->px->text().toDouble() != bar_->x()) ||
      (ui->py->text().toDouble() != bar_->y())) {
    bar_->setPos(ui->px->text().toDouble(), ui->py->text().toDouble());
  }

  // Add Bar to project.
  if (isNew) {
    if(!network->addBar(bar_)) {
      QMessageBox::critical(this, "Invalid Bar", "Can't add new bar to project.",
                            QMessageBox::Ok);
      delete bar_;
      reject();
    }
  }

  accept();
}

/*******************************************************************************
 * Botton Box Rejected.
 ******************************************************************************/
void BarProperties::on_buttonBox_rejected()
{
  reject();
}

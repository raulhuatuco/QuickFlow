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

  ui->Sga->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Sgb->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Sgc->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Sgai->setValidator(new QDoubleValidator(this));
  ui->Sgbi->setValidator(new QDoubleValidator(this));
  ui->Sgci->setValidator(new QDoubleValidator(this));

  ui->Sla->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Slb->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Slc->setValidator(new QDoubleValidator(0,qInf(), 1000, this));
  ui->Slai->setValidator(new QDoubleValidator(this));
  ui->Slbi->setValidator(new QDoubleValidator(this));
  ui->Slci->setValidator(new QDoubleValidator(this));

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
 * setBar.
 ******************************************************************************/
void BarProperties::setBar(PnBar *bar, bool newBar)
{
  // Adjust apearance according to bar.
  if (newBar) {
    setWindowTitle(tr("New Bar"));
  } else {
    setWindowTitle(tr("Edit Bar ") + QString::number(bar->id));
    ui->id->setEnabled(false);
  }

  // Store parameters.
  bar_ = bar;

  // Fill Bar data.
  ui->id->setValue(bar->id);
  ui->Va->setText(QString::number(bar->Va.real()));
  ui->Vb->setText(QString::number(bar->Vb.real()));
  ui->Vc->setText(QString::number(bar->Vc.real()));
  ui->Vai->setText(QString::number(bar->Va.imag()));
  ui->Vbi->setText(QString::number(bar->Vb.imag()));
  ui->Vci->setText(QString::number(bar->Vc.imag()));
  ui->Sga->setText(QString::number(bar->Sga.real()));
  ui->Sgb->setText(QString::number(bar->Sgb.real()));
  ui->Sgc->setText(QString::number(bar->Sgc.real()));
  ui->Sgai->setText(QString::number(bar->Sga.imag()));
  ui->Sgbi->setText(QString::number(bar->Sgb.imag()));
  ui->Sgci->setText(QString::number(bar->Sgc.imag()));
  ui->Sla->setText(QString::number(bar->Sla.real()));
  ui->Slb->setText(QString::number(bar->Slb.real()));
  ui->Slc->setText(QString::number(bar->Slc.real()));
  ui->Slai->setText(QString::number(bar->Sla.imag()));
  ui->Slbi->setText(QString::number(bar->Slb.imag()));
  ui->Slci->setText(QString::number(bar->Slc.imag()));
  ui->px->setText(QString::number(bar->x()));
  ui->py->setText(QString::number(bar->y()));
}

/*******************************************************************************
 * setUnit.
 ******************************************************************************/
void BarProperties::setUnit(Unit::VoltageUnit voltageUnit,
                            Unit::PowerUnit powerUnit)
{
  // Voltage.
  ui->VaUn->setText(tr("[") + Unit::voltageUnitToStr(voltageUnit) + tr("]"));
  ui->VbUn->setText(tr("[") + Unit::voltageUnitToStr(voltageUnit) + tr("]"));
  ui->VcUn->setText(tr("[") + Unit::voltageUnitToStr(voltageUnit) + tr("]"));

  // Power.
  ui->SgaUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
  ui->SlaUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
  ui->SgbUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
  ui->SlbUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
  ui->SgcUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
  ui->SlcUn->setText(tr("[") + Unit::powerUnitToStr(powerUnit) + tr("]"));
}

/*******************************************************************************
 * Botton Box Accepted.
 ******************************************************************************/
void BarProperties::on_buttonBox_accepted()
{
  // Check for empty fields.
  // Voltages.
  // Real Part.
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
  if (ui->Sga->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sga is empty.",
                             QMessageBox::Ok);
    ui->Sga->setFocus();
    return;
  }

  if (ui->Sgb->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sgb is empty.",
                             QMessageBox::Ok);
    ui->Sgb->setFocus();
    return;
  }

  if (ui->Sgc->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sgc is empty.",
                             QMessageBox::Ok);
    ui->Sgc->setFocus();
    return;
  }

  // Imaginary Part.
  if (ui->Sgai->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sga is empty.",
                             QMessageBox::Ok);
    ui->Sgai->setFocus();
    return;
  }

  if (ui->Sgbi->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sgb is empty.",
                             QMessageBox::Ok);
    ui->Sgbi->setFocus();
    return;
  }

  if (ui->Sgci->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sgc is empty.",
                             QMessageBox::Ok);
    ui->Sgci->setFocus();
    return;
  }

  // Consumed Power.
  // Real Part.
  if (ui->Sla->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sla is empty.",
                             QMessageBox::Ok);
    ui->Sla->setFocus();
    return;
  }

  if (ui->Slb->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Slb is empty.",
                             QMessageBox::Ok);
    ui->Slb->setFocus();
    return;
  }

  if (ui->Slc->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Slc is empty.",
                             QMessageBox::Ok);
    ui->Slc->setFocus();
    return;
  }

  // Imaginary Part.
  if (ui->Slai->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Sla is empty.",
                             QMessageBox::Ok);
    ui->Slai->setFocus();
    return;
  }

  if (ui->Slbi->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Slb is empty.",
                             QMessageBox::Ok);
    ui->Slbi->setFocus();
    return;
  }

  if (ui->Slci->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter Slc is empty.",
                             QMessageBox::Ok);
    ui->Slci->setFocus();
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

  // Voltage.
  bar_->Va.real(ui->Va->text().toDouble());
  bar_->Vb.real(ui->Vb->text().toDouble());
  bar_->Vc.real(ui->Vc->text().toDouble());
  bar_->Va.imag(ui->Vai->text().toDouble());
  bar_->Vb.imag(ui->Vbi->text().toDouble());
  bar_->Vc.imag(ui->Vci->text().toDouble());

  // Generated power.
  bar_->Sga.real(ui->Sga->text().toDouble());
  bar_->Sgb.real(ui->Sgb->text().toDouble());
  bar_->Sgc.real(ui->Sgc->text().toDouble());
  bar_->Sga.imag(ui->Sgai->text().toDouble());
  bar_->Sgb.imag(ui->Sgbi->text().toDouble());
  bar_->Sgc.imag(ui->Sgci->text().toDouble());

  // Consumed power.
  bar_->Sla.real(ui->Sla->text().toDouble());
  bar_->Slb.real(ui->Slb->text().toDouble());
  bar_->Slc.real(ui->Slc->text().toDouble());
  bar_->Sla.imag(ui->Slai->text().toDouble());
  bar_->Slb.imag(ui->Slbi->text().toDouble());
  bar_->Slc.imag(ui->Slci->text().toDouble());

  // Position
  if((ui->px->text().toDouble() != bar_->x()) ||
      (ui->py->text().toDouble() != bar_->y())) {
    bar_->setPos(ui->px->text().toDouble(), ui->py->text().toDouble());
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
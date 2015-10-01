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
  } else {
    setWindowTitle(tr("Edit Bar ") + QString::number(bar->id));
    ui->id->setEnabled(false);
    // Store parameters.
    bar_ = bar;
    ui->id->setValue(bar_->id);
    isNew = false;
  }

  // Store project parameters.
  project_ = project;

  // Fill Bar data.
  ui->Va->setText(QString::number(bar_->Va.real()));
  ui->Vb->setText(QString::number(bar_->Vb.real()));
  ui->Vc->setText(QString::number(bar_->Vc.real()));
  ui->Vai->setText(QString::number(bar_->Va.imag()));
  ui->Vbi->setText(QString::number(bar_->Vb.imag()));
  ui->Vci->setText(QString::number(bar_->Vc.imag()));
  ui->Sha->setText(QString::number(bar_->Sha.real()));
  ui->Shb->setText(QString::number(bar_->Shb.real()));
  ui->Shc->setText(QString::number(bar_->Shc.real()));
  ui->Shai->setText(QString::number(bar_->Sha.imag()));
  ui->Shbi->setText(QString::number(bar_->Shb.imag()));
  ui->Shci->setText(QString::number(bar_->Shc.imag()));
  ui->Sia->setText(QString::number(bar_->Sia.real()));
  ui->Sib->setText(QString::number(bar_->Sib.real()));
  ui->Sic->setText(QString::number(bar_->Sic.real()));
  ui->Siai->setText(QString::number(bar_->Sia.imag()));
  ui->Sibi->setText(QString::number(bar_->Sib.imag()));
  ui->Sici->setText(QString::number(bar_->Sic.imag()));
  ui->px->setText(QString::number(bar_->x()));
  ui->py->setText(QString::number(bar_->y()));

  // Set units.
  // Voltage.
  ui->VaUn->setText(tr("[") + Unit::voltageUnitToStr(project->voltageUnit()) +
                    tr("]"));
  ui->VbUn->setText(tr("[") + Unit::voltageUnitToStr(project->voltageUnit()) +
                    tr("]"));
  ui->VcUn->setText(tr("[") + Unit::voltageUnitToStr(project->voltageUnit()) +
                    tr("]"));

  // Power.
  ui->ShaUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
  ui->SiaUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
  ui->ShbUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
  ui->SibUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
  ui->ShcUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
  ui->SicUn->setText(tr("[") + Unit::powerUnitToStr(project->powerUnit()) +
                     tr("]"));
}

/*******************************************************************************
 * Botton Box Accepted.
 ******************************************************************************/
void BarProperties::on_buttonBox_accepted()
{
  // Check for valid id.
  if(isNew) {
    if (project_->network->getBarById(ui->id->value()) != NULL) {
      QMessageBox::information(this, "Invalid parameter", "Id already in use.",
                               QMessageBox::Ok);
      ui->id->setFocus();
      return;
    }
  }

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

  // Voltage.
  bar_->Va.real(ui->Va->text().toDouble());
  bar_->Vb.real(ui->Vb->text().toDouble());
  bar_->Vc.real(ui->Vc->text().toDouble());
  bar_->Va.imag(ui->Vai->text().toDouble());
  bar_->Vb.imag(ui->Vbi->text().toDouble());
  bar_->Vc.imag(ui->Vci->text().toDouble());

  // Generated power.
  bar_->Sha.real(ui->Sha->text().toDouble());
  bar_->Shb.real(ui->Shb->text().toDouble());
  bar_->Shc.real(ui->Shc->text().toDouble());
  bar_->Sha.imag(ui->Shai->text().toDouble());
  bar_->Shb.imag(ui->Shbi->text().toDouble());
  bar_->Shc.imag(ui->Shci->text().toDouble());

  // Consumed power.
  bar_->Sia.real(ui->Sia->text().toDouble());
  bar_->Sib.real(ui->Sib->text().toDouble());
  bar_->Sic.real(ui->Sic->text().toDouble());
  bar_->Sia.imag(ui->Siai->text().toDouble());
  bar_->Sib.imag(ui->Sibi->text().toDouble());
  bar_->Sic.imag(ui->Sici->text().toDouble());

  // Position
  if((ui->px->text().toDouble() != bar_->x()) ||
      (ui->py->text().toDouble() != bar_->y())) {
    bar_->setPos(ui->px->text().toDouble(), ui->py->text().toDouble());
  }

  // Add Bar to project.
  if (isNew) {
    if(!project_->network->addBar(bar_)) {
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

#include "lineproperties.h"
#include "ui_lineproperties.h"
#include <QMessageBox>

/*******************************************************************************
 * LineProperties.
 ******************************************************************************/
LineProperties::LineProperties(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LineProperties)
{
  ui->setupUi(this);

  // Add length validator.
  ui->length->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
}

/*******************************************************************************
 * ~LineProperties.
 ******************************************************************************/
LineProperties::~LineProperties()
{
  delete ui;
}

/*******************************************************************************
 * setOptions.
 ******************************************************************************/
void LineProperties::setOptions(Project *project, Line *line)
{
  // Adjust apearance according to line.
  if (line == NULL) {
    setWindowTitle(tr("New Line"));
    line_ = new Line;
    isNew = true;

    // Fill line ids combobox.
    foreach (Bar *bar, project->network->bars) {
      ui->noI->addItem(QString::number(bar->id));
      ui->noF->addItem(QString::number(bar->id));
    }

  } else {
    setWindowTitle(tr("Edit Line from node ") + QString::number(
                     line->noI) + tr(" to node ") \
                   + QString::number(line->noF));
    line_ = line;
    isNew = false;

    // Fill line ids combobox.
    ui->noI->addItem(QString::number(line->noI));
    ui->noF->addItem(QString::number(line->noF));
    ui->noI->setEnabled(false);
    ui->noF->setEnabled(false);
  }

  // Store project parameters.
  project_ = project;

  // Store parameters.
  // Length.
  ui->length->setText(QString::number(line_->length));

  // Impedance.
  ui->Zaa->setText(QString::number(line_->Zaa.real()));
  ui->Zaai->setText(QString::number(line_->Zaa.imag()));
  ui->Zab->setText(QString::number(line_->Zab.real()));
  ui->Zabi->setText(QString::number(line_->Zab.imag()));
  ui->Zac->setText(QString::number(line_->Zac.real()));
  ui->Zaci->setText(QString::number(line_->Zac.imag()));
  ui->Zbb->setText(QString::number(line_->Zbb.real()));
  ui->Zbbi->setText(QString::number(line_->Zbb.imag()));
  ui->Zbc->setText(QString::number(line_->Zbc.real()));
  ui->Zbci->setText(QString::number(line_->Zbc.imag()));
  ui->Zcc->setText(QString::number(line_->Zcc.real()));
  ui->Zcci->setText(QString::number(line_->Zcc.imag()));

  // Set units.
  // Length.
  ui->lengthUn->setText(tr("[") + Unit::lengthUnitToStr(project->lengthUnit()) +
                        tr("]"));

  // Impedance.
  ui->impedanceUn1->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
  ui->impedanceUn2->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
  ui->impedanceUn3->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
  ui->impedanceUn5->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
  ui->impedanceUn6->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
  ui->impedanceUn8->setText(tr("[") + Unit::impedanceUnitToStr(
                              project->impedanceUnit()) + tr("]"));
}

/*******************************************************************************
 * Botton Box Accepted.
 ******************************************************************************/
void LineProperties::on_buttonBox_accepted()
{
  // Check for valid data.
  // Nodes.
  if(ui->noI->currentText() == ui->noF->currentText()) {
    QMessageBox::information(this, "Invalid parameter",
                             "Initial and final nodes must be diferent.",
                             QMessageBox::Ok);
    ui->noI->setFocus();
    return;
  }

  // Check if line already exists for new lines.
  if(isNew) {
    if(project_->network->getLineByNodes(ui->noI->currentText().toInt(),
                                         ui->noF->currentText().toInt()) != NULL) {
      QMessageBox::information(this, "Invalid parameter",
                               "Line already exist.",
                               QMessageBox::Ok);
      ui->noI->setFocus();
      return;
    }
  }

  // Length.
  if (ui->length->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter",
                             "Parameter Length is empty.",
                             QMessageBox::Ok);
    ui->length->setFocus();
    return;
  }

  // Impedance.
  //Zaa.
  if (!validImpedance(ui->Zaa))
    return;

  //Zaai.
  if (!validImpedance(ui->Zaai))
    return;

  //Zab.
  if (!validImpedance(ui->Zab))
    return;

  //Zabi.
  if (!validImpedance(ui->Zabi))
    return;

  //Zac.
  if (!validImpedance(ui->Zac))
    return;

  //Zaci.
  if (!validImpedance(ui->Zaci))
    return;

  //Zbb.
  if (!validImpedance(ui->Zbb))
    return;

  //Zbbi.
  if (!validImpedance(ui->Zbbi))
    return;

  //Zbc.
  if (!validImpedance(ui->Zbc))
    return;

  //Zbci.
  if (!validImpedance(ui->Zbci))
    return;

  //Zcc.
  if (!validImpedance(ui->Zcc))
    return;

  //Zcci.
  if (!validImpedance(ui->Zcci))
    return;

  // Set nodes if it's a new line.
  if(isNew) {
    // Set noI and noF.
    line_->noI = ui->noI->currentText().toInt();
    line_->noF = ui->noF->currentText().toInt();
  }

  // Set impedance.

  line_->Zaa.real(ui->Zaa->text().toDouble());
  line_->Zaa.imag(ui->Zaai->text().toDouble());

  line_->Zab.real(ui->Zab->text().toDouble());
  line_->Zab.imag(ui->Zabi->text().toDouble());

  line_->Zac.real(ui->Zac->text().toDouble());
  line_->Zac.imag(ui->Zaci->text().toDouble());

  line_->Zbb.real(ui->Zbb->text().toDouble());
  line_->Zbb.imag(ui->Zbbi->text().toDouble());

  line_->Zbc.real(ui->Zbc->text().toDouble());
  line_->Zbc.imag(ui->Zbci->text().toDouble());

  line_->Zcc.real(ui->Zcc->text().toDouble());
  line_->Zcc.imag(ui->Zcci->text().toDouble());

  line_->length = ui->length->text().toDouble();

  // Add new line to network.
  if (isNew) {
    if(!project_->network->addLine(line_)) {
      QMessageBox::critical(this, "Invalid Line", "Can't add new line to project.",
                            QMessageBox::Ok);
      delete line_;
      reject();
    }
  }

  accept();
}

/*******************************************************************************
 * Botton Box Rejected.
 ******************************************************************************/
void LineProperties::on_buttonBox_rejected()
{
  reject();
}

/*******************************************************************************
 * validImpedance.
 ******************************************************************************/
bool LineProperties::validImpedance(QLineEdit *input)
{
  bool isDouble;
  input->text().toDouble(&isDouble);

  if (input->text().isEmpty() || !isDouble) {

    QMessageBox::information(this, "Invalid parameter",
                             "Parameter " + input->objectName() + " is invalid.",
                             QMessageBox::Ok);
    input->setFocus();
    return false;
  } else {
    return true;
  }
}

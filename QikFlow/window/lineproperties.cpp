#include "lineproperties.h"
#include "ui_lineproperties.h"

#include <QString>
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
 * setLine.
 ******************************************************************************/
void LineProperties::setLine(PnLine *line, bool newLine)
{
// Adjust apearance according to line.
  if (newLine) {
    setWindowTitle(tr("New Line"));

  } else {
    setWindowTitle(tr("Edit Line from node ") + QString::number(
                     line->noI()->id) + tr(" to node ") \
                   + QString::number(line->noF()->id));

    ui->noI->addItem(QString::number(line->noI()->id));
    ui->noF->addItem(QString::number(line->noF()->id));
    ui->noI->setEnabled(false);
    ui->noF->setEnabled(false);
  }

  // Store parameters.
  // Length.
  ui->length->setText(QString::number(line->length));

  // Impedance.
  ui->Zaa->setText(QString::number(line->Zaa.real()));
  ui->Zaai->setText(QString::number(line->Zaa.imag()));
  ui->Zab->setText(QString::number(line->Zab.real()));
  ui->Zabi->setText(QString::number(line->Zab.imag()));
  ui->Zac->setText(QString::number(line->Zac.real()));
  ui->Zaci->setText(QString::number(line->Zac.imag()));
  ui->Zan->setText(QString::number(line->Zan.real()));
  ui->Zani->setText(QString::number(line->Zan.imag()));
  ui->Zbb->setText(QString::number(line->Zbb.real()));
  ui->Zbbi->setText(QString::number(line->Zbb.imag()));
  ui->Zbc->setText(QString::number(line->Zbc.real()));
  ui->Zbci->setText(QString::number(line->Zbc.imag()));
  ui->Zbn->setText(QString::number(line->Zbn.real()));
  ui->Zbni->setText(QString::number(line->Zbn.imag()));
  ui->Zcc->setText(QString::number(line->Zcc.real()));
  ui->Zcci->setText(QString::number(line->Zcc.imag()));
  ui->Zcn->setText(QString::number(line->Zcn.real()));
  ui->Zcni->setText(QString::number(line->Zcn.imag()));
  ui->Znn->setText(QString::number(line->Znn.real()));
  ui->Znni->setText(QString::number(line->Znn.imag()));

  // Save line address
  line_ = line;

  newLine_ = newLine;
}

/*******************************************************************************
 * setBarMap.
 ******************************************************************************/
void LineProperties::setBarMap(QMap<uint32_t, PnBar *> &barMap)
{
  foreach (PnBar *bar, barMap) {
    ui->noI->addItem(QString::number(bar->id));
    ui->noF->addItem(QString::number(bar->id));
  }

  barMap_ = &barMap;
}

/*******************************************************************************
 * setUnit.
 ******************************************************************************/
void LineProperties::setUnit(Unit::LengthUnit lengthUnit,
                             Unit::ImpedanceUnit impedanceUnit)
{
  // Length.
  ui->lengthUn->setText(tr("[") + Unit::lengthUnitToStr(lengthUnit) + tr("]"));

  // Impedance.
  ui->impedanceUn1->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn2->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn3->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn4->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn5->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn6->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn7->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn8->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn9->setText(tr("[") + Unit::impedanceUnitToStr(
                              impedanceUnit) + tr("]"));
  ui->impedanceUn10->setText(tr("[") + Unit::impedanceUnitToStr(
                               impedanceUnit) + tr("]"));
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

  //Zan.
  if (!validImpedance(ui->Zan))
    return;

  //Zani.
  if (!validImpedance(ui->Zani))
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

  //Zbn.
  if (!validImpedance(ui->Zbn))
    return;

  //Zbni.
  if (!validImpedance(ui->Zbni))
    return;

  //Zcc.
  if (!validImpedance(ui->Zcc))
    return;

  //Zcci.
  if (!validImpedance(ui->Zcci))
    return;

  //Zcn.
  if (!validImpedance(ui->Zcn))
    return;

  //Zcni.
  if (!validImpedance(ui->Zcni))
    return;

  //Znn.
  if (!validImpedance(ui->Znn))
    return;

  //Znni.
  if (!validImpedance(ui->Znni))
    return;

  if(newLine_) {
    PnBar *NodeI = barMap_->value(ui->noI->currentText().toInt(), NULL);
    PnBar *NodeF = barMap_->value(ui->noF->currentText().toInt(), NULL);
    // Set noI and noF.
    line_->setNodes(NodeI, NodeF);
  }

  // Set impedance.
  line_->Zaa.real(ui->Zaa->text().toDouble());
  line_->Zaa.imag(ui->Zaai->text().toDouble());

  line_->Zab.real(ui->Zab->text().toDouble());
  line_->Zab.imag(ui->Zabi->text().toDouble());

  line_->Zac.real(ui->Zac->text().toDouble());
  line_->Zac.imag(ui->Zaci->text().toDouble());

  line_->Zan.real(ui->Zan->text().toDouble());
  line_->Zan.imag(ui->Zani->text().toDouble());

  line_->Zbb.real(ui->Zbb->text().toDouble());
  line_->Zbb.imag(ui->Zbbi->text().toDouble());

  line_->Zbc.real(ui->Zbc->text().toDouble());
  line_->Zbc.imag(ui->Zbci->text().toDouble());

  line_->Zbn.real(ui->Zbn->text().toDouble());
  line_->Zbn.imag(ui->Zbni->text().toDouble());

  line_->Zcc.real(ui->Zcc->text().toDouble());
  line_->Zcc.imag(ui->Zcci->text().toDouble());

  line_->Zcn.real(ui->Zcn->text().toDouble());
  line_->Zcn.imag(ui->Zcni->text().toDouble());

  line_->Znn.real(ui->Znn->text().toDouble());
  line_->Znn.imag(ui->Znni->text().toDouble());

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

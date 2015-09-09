#include "lineproperties.h"
#include "ui_lineproperties.h"

#include <QString>

LineProperties::LineProperties(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LineProperties) {
  ui->setupUi(this);

  // Add length validator.
  ui->length->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
}

LineProperties::~LineProperties() {
  delete ui;
}

void LineProperties::setLine(PnLine *line, bool newLine) {
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


}

void LineProperties::setBarMap(QMap<uint32_t, PnBar *> &barMap) {
  foreach (PnBar *bar, barMap) {
    ui->noI->addItem(QString::number(bar->id));
    ui->noF->addItem(QString::number(bar->id));
  }
}

void LineProperties::setUnit(Unit::LengthUnit lengthUnit,
                             Unit::ImpedanceUnit impedanceUnit) {
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

void LineProperties::on_buttonBox_accepted() {
  
  
  accept();
}

void LineProperties::on_buttonBox_rejected() {
  reject();
}

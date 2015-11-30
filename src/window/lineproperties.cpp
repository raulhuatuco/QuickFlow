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
bool LineProperties::setOptions(Project *project, Line *line)
{

  // Check if project contains a network.
  if(project->networks.isEmpty()) {
    QMessageBox::critical(this, "Not networks found", "First create a new network.",
                          QMessageBox::Ok);
    return false;
  }

  // Adjust apearance according to line.
  if (line == NULL) {
    setWindowTitle(tr("New Line"));
    line_ = new Line;
    isNew = true;

    foreach(Network *network, project->networks) {
      ui->network->addItem(network->name);
    }

    Network *network = project->networks.value(ui->network->currentText(),NULL);

    // Fill bar ids combobox.
    foreach (Bar *bar, network->bars) {
      ui->noI->addItem(QString::number(bar->id()));
      ui->noF->addItem(QString::number(bar->id()));
    }

    line_->setNetwork(network);
  } else {
    setWindowTitle(tr("Edit Line from node ") + QString::number(
                     line->nodes.first) + tr(" to node ") \
                   + QString::number(line->nodes.second));
    line_ = line;
    isNew = false;

    ui->network->addItem(line->network()->name);
    ui->network->setEnabled(false);

    // Fill line ids combobox.
    ui->noI->addItem(QString::number(line->nodes.first));
    ui->noF->addItem(QString::number(line->nodes.second));
    ui->noI->setEnabled(false);
    ui->noF->setEnabled(false);
  }

  // Store project parameters.
  project_ = project;

  // Store parameters.
  // Length.
  ui->length->setText(QString::number(line_->length(project->lengthUnit())));

  // Impedance.
  ui->Zaa->setText(QString::number(line_->z(0,project->impedanceUnit()).real()));
  ui->Zaai->setText(QString::number(line_->z(0,project->impedanceUnit()).imag()));
  ui->Zab->setText(QString::number(line_->z(1,project->impedanceUnit()).real()));
  ui->Zabi->setText(QString::number(line_->z(1,project->impedanceUnit()).imag()));
  ui->Zac->setText(QString::number(line_->z(2,project->impedanceUnit()).real()));
  ui->Zaci->setText(QString::number(line_->z(2,project->impedanceUnit()).imag()));
  ui->Zbb->setText(QString::number(line_->z(3,project->impedanceUnit()).real()));
  ui->Zbbi->setText(QString::number(line_->z(3,project->impedanceUnit()).imag()));
  ui->Zbc->setText(QString::number(line_->z(4,project->impedanceUnit()).real()));
  ui->Zbci->setText(QString::number(line_->z(4,project->impedanceUnit()).imag()));
  ui->Zcc->setText(QString::number(line_->z(5,project->impedanceUnit()).real()));
  ui->Zcci->setText(QString::number(line_->z(5,project->impedanceUnit()).imag()));

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

  return true;
}

Line *LineProperties::line()
{
  return line_;
}

/*******************************************************************************
 * Botton Box Accepted.
 ******************************************************************************/
void LineProperties::on_buttonBox_accepted()
{

  Network *network = project_->networks.value(ui->network->currentText(), NULL);

  if (network == NULL) {
    QMessageBox::information(this, "Invalid parameter", "Invalid network.",
                             QMessageBox::Ok);
    ui->network->setFocus();
    return;
  }

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
  QPair<int32_t, int32_t> nodes(ui->noI->currentText().toInt(),
                                ui->noF->currentText().toInt());

  if(isNew) {
    if(network->getLineByNodes(nodes) != NULL) {
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

  // Set nodes and network if it's a new line.
  if(isNew) {
    // Set noI and noF.
    line_->nodes = nodes;
    // Set network.
    line_->setNetwork(network);
  }

  // Set length
  line_->setLength(ui->length->text().toDouble(), project_->lengthUnit());

  // Set impedance.
  complex<double> z;

  z.real(ui->Zaa->text().toDouble());
  z.imag(ui->Zaai->text().toDouble());
  line_->setZ(0, z, project_->impedanceUnit());

  z.real(ui->Zab->text().toDouble());
  z.imag(ui->Zabi->text().toDouble());
  line_->setZ(1, z, project_->impedanceUnit());

  z.real(ui->Zac->text().toDouble());
  z.imag(ui->Zaci->text().toDouble());
  line_->setZ(2, z, project_->impedanceUnit());

  z.real(ui->Zbb->text().toDouble());
  z.imag(ui->Zbbi->text().toDouble());
  line_->setZ(3, z, project_->impedanceUnit());

  z.real(ui->Zbc->text().toDouble());
  z.imag(ui->Zbci->text().toDouble());
  line_->setZ(4, z, project_->impedanceUnit());

  z.real(ui->Zcc->text().toDouble());
  z.imag(ui->Zcci->text().toDouble());
  line_->setZ(5, z, project_->impedanceUnit());

  // Add new line to project.
  if (isNew) {
    if(!network->addLine(line_)) {
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

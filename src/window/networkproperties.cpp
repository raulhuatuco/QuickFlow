#include "networkproperties.h"
#include "ui_networkproperties.h"

#include <QColorDialog>
#include <QMessageBox>
#include <QValidator>

#include "math_constants.h"

NetworkProperties::NetworkProperties(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::NetworkProperties),
  project_(NULL),
  network_(NULL),
  isNew(false),
  slack(Qt::green),
  pq(Qt::blue),
  barBorder(Qt::gray),
  line(Qt::black)
{
  ui->setupUi(this);

  // Add data validators.
  ui->xOffset->setValidator(new QDoubleValidator(this));
  ui->yOffset->setValidator(new QDoubleValidator(this));

  ui->voltageBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
  ui->powerBase->setValidator(new QDoubleValidator(0, qInf(), 1000, this));

}

NetworkProperties::~NetworkProperties()
{
  delete ui;
}

void NetworkProperties::setOptions(Project *project, Network *network)
{
// Adjust apearance according to network.
  if (network == NULL) {
    setWindowTitle(tr("New network"));
    network_ = new Network;
    isNew = true;

  } else {
    setWindowTitle(tr("Edit network ") + network->name);
    network_ = network;
    isNew = false;

    // Initialize colors from network.
    QString slackColor =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(
        network->barSlackFillColor.name()).arg(network->barSlackFillColor.name());
    ui->slackColor->setStyleSheet(slackColor);

    QString pqColor =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(
        network->barPqFillColor.name()).arg(network->barPqFillColor.name());
    ui->pqColor->setStyleSheet(pqColor);

    QString lineColor =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(
        network->lineColor.name()).arg(network->lineColor.name());
    ui->lineColor->setStyleSheet(lineColor);

    QString borderColor =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(
        network->barStrokeColor.name()).arg(network->barStrokeColor.name());
    ui->barBorderColor->setStyleSheet(borderColor);

    ui->voltageBase->setText(QString::number(network->voltageBase()*kSQRT3));
    ui->powerBase->setText(QString::number(network->powerBase()*3.0));
  }

  // Store project parameters.
  project_ = project;
}

Network *NetworkProperties::network()
{
  return network_;
}

void NetworkProperties::on_slackColor_clicked()
{
  QColor col = QColorDialog::getColor(Qt::green, this, "Select Slack bar color",
                                      QColorDialog::DontUseNativeDialog);

  if(col.isValid()) {
    QString qss =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(col.name()).arg(col.name());
    ui->slackColor->setStyleSheet(qss);
    slack = col;
  }
}

void NetworkProperties::on_pqColor_clicked()
{
  QColor col = QColorDialog::getColor(Qt::blue, this, "Select PQ bar color",
                                      QColorDialog::DontUseNativeDialog);

  if(col.isValid()) {
    QString qss =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(col.name()).arg(col.name());
    ui->pqColor->setStyleSheet(qss);
    pq = col;
  }
}

void NetworkProperties::on_barBorderColor_clicked()
{
  QColor col = QColorDialog::getColor(Qt::gray, this, "Select bar border color",
                                      QColorDialog::DontUseNativeDialog);

  if(col.isValid()) {
    QString qss =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(col.name()).arg(col.name());
    ui->barBorderColor->setStyleSheet(qss);
    barBorder = col;
  }
}

void NetworkProperties::on_lineColor_clicked()
{
  QColor col = QColorDialog::getColor(Qt::black, this, "Select line color",
                                      QColorDialog::DontUseNativeDialog);

  if(col.isValid()) {
    QString qss =
      QString("border: 2px solid #000000;" \
              "\nborder-radius: 6px;" \
              "\nbackground-color: %1;" \
              "\nselection-background-color: %2;").arg(col.name()).arg(col.name());
    ui->lineColor->setStyleSheet(qss);
    line = col;
  }
}

void NetworkProperties::on_buttonBox_accepted()
{
  if(ui->networkName->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Network name is empty.",
                             QMessageBox::Ok);
    ui->networkName->setFocus();
    return;
  }

  if(ui->xOffset->text().isEmpty()) {
    ui->xOffset->setText("0");
  }

  if(ui->yOffset->text().isEmpty()) {
    ui->yOffset->setText("0");
  }

  // Check for empty voltage base.
  if(ui->voltageBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid voltage base",
                             "Select a voltage base.",
                             QMessageBox::Ok);
    ui->voltageBase->setFocus();
    return;
  }

  // Check for empty power base.
  if(ui->powerBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid power base",
                             "Select a power base.",
                             QMessageBox::Ok);
    ui->powerBase->setFocus();
    return;
  }

  network_->name = ui->networkName->text();
  network_->barSlackFillColor = slack;
  network_->barPqFillColor = pq;
  network_->barStrokeColor = barBorder;
  network_->lineColor = line;
  network_->xOffset = ui->xOffset->text().toDouble();
  network_->yOffset = ui->yOffset->text().toDouble();

  network_->setVoltageBase(ui->voltageBase->text().toDouble()/kSQRT3);
  network_->setPowerBase(ui->powerBase->text().toDouble()/3.0);

  if(isNew)
    project_->networks.insert(network_->name, network_);

  accept();
}

void NetworkProperties::on_buttonBox_rejected()
{
  reject();
}

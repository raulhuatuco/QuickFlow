#include "networkproperties.h"
#include "ui_networkproperties.h"

#include <QColorDialog>
#include <QMessageBox>
#include <QValidator>

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
    setWindowTitle(tr("Edit network "));
    network_ = network;
    isNew = false;
    // Todo: initialize colors from network.
  }

  // Store project parameters.
  project_ = project;
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

  network_->name = ui->networkName->text();
  network_->barSlackFillColor = slack;
  network_->barPqFillColor = pq;
  network_->barStrokeColor = barBorder;
  network_->lineColor = line;
  network_->xOffset = ui->xOffset->text().toDouble();
  network_->yOffset = ui->yOffset->text().toDouble();

  if(isNew)
    project_->networks.insert(network_->name, network_);

  accept();
}

void NetworkProperties::on_buttonBox_rejected()
{
  reject();
}

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \addtogroup Window
 * \{
 */

/*!
 * \file networkproperties.cpp
 *
 * \brief Implementation of the NetworkProperties class.
 *
 * This file contains the implementation for the NetworkProperties class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "networkproperties.h"
#include "ui_networkproperties.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QValidator>
#include "math_constants.h"

/*******************************************************************************
 * Constructor.
 ******************************************************************************/
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

/*******************************************************************************
 * Destructor.
 ******************************************************************************/
NetworkProperties::~NetworkProperties()
{
  delete ui;
}

/*******************************************************************************
 * Set options.
 ******************************************************************************/
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

    // Initialize bases.
    // Note that the input value is the line voltage, so we neeed to multiply
    // the stored voltage base by sqrt(3), because the software uses the phase
    //  voltage in the calculations:
    // Vline = Vline*sqrt(3);
    ui->voltageBase->setText(QString::number(network->voltageBase()*kSQRT3));

    // Note that the power base input is the total power. We use the power per
    // fase, so, to display the correct value we need to get the total power,
    // wich is:
    // Ptotal = Pphase*3.0;
    ui->powerBase->setText(QString::number(network->powerBase()*3.0));
  }

  // Store project parameters.
  project_ = project;
}

/*******************************************************************************
 * Network pointer.
 ******************************************************************************/
Network *NetworkProperties::network()
{
  return network_;
}

/*******************************************************************************
 * Slack color picker box clicked.
 ******************************************************************************/
void NetworkProperties::on_slackColor_clicked()
{
  // Open color picker dialog.
  QColor col = QColorDialog::getColor(Qt::green, this, "Select Slack bar color",
                                      QColorDialog::DontUseNativeDialog);

  // Set the color picker box color
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

/*******************************************************************************
 * PQ color picker box clicked.
 ******************************************************************************/
void NetworkProperties::on_pqColor_clicked()
{
  // Open color picker dialog.
  QColor col = QColorDialog::getColor(Qt::blue, this, "Select PQ bar color",
                                      QColorDialog::DontUseNativeDialog);

  // Set the color picker box color.
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

/*******************************************************************************
 * Border color picker box clicked.
 ******************************************************************************/
void NetworkProperties::on_barBorderColor_clicked()
{
  // Open color picker dialog.
  QColor col = QColorDialog::getColor(Qt::gray, this, "Select bar border color",
                                      QColorDialog::DontUseNativeDialog);

  // Set the color picker box color.
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

/*******************************************************************************
 * Line color picker box clicked.
 ******************************************************************************/
void NetworkProperties::on_lineColor_clicked()
{
  // Open color picker dialog.
  QColor col = QColorDialog::getColor(Qt::black, this, "Select line color",
                                      QColorDialog::DontUseNativeDialog);

  // Set the color picker box color.
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

/*******************************************************************************
 * Button accept.
 ******************************************************************************/
void NetworkProperties::on_buttonBox_accepted()
{
  // Check for empty fields.
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

  if(ui->voltageBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid voltage base",
                             "Select a voltage base.",
                             QMessageBox::Ok);
    ui->voltageBase->setFocus();
    return;
  }

  if(ui->powerBase->text().isEmpty()) {
    QMessageBox::information(this, "Invalid power base",
                             "Select a power base.",
                             QMessageBox::Ok);
    ui->powerBase->setFocus();
    return;
  }

  // Fill network data.
  network_->name = ui->networkName->text();
  network_->barSlackFillColor = slack;
  network_->barPqFillColor = pq;
  network_->barStrokeColor = barBorder;
  network_->lineColor = line;
  network_->xOffset = ui->xOffset->text().toDouble();
  network_->yOffset = ui->yOffset->text().toDouble();

  // Note that the input value is the line voltage, so we neeed to divide it
  // by sqrt(3), because the software uses the phase voltage in the
  // calculations:
  // Vphase = Vline/sqrt(3);
  network_->setVoltageBase(ui->voltageBase->text().toDouble()/kSQRT3);


  // Note that the power base input is the total power. We use the power per
  // fase, so, since we have 3 phases, Pphase = Ptotal/3.0;
  network_->setPowerBase(ui->powerBase->text().toDouble()/3.0);

  // If new, we need to insert the network into the project.
  if(isNew)
    project_->networks.insert(network_->name, network_);

  accept();
}

/*******************************************************************************
 * Button reject.
 ******************************************************************************/
void NetworkProperties::on_buttonBox_rejected()
{
  reject();
}

/*!
 * \}
 */

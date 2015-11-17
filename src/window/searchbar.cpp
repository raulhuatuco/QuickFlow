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
 * \file searchbar.cpp
 *
 * \brief Implementation of SearchBar class.
 *
 * This is the implementation of the SearchBar class.
 *
 * \author David Krepsky
 * \version 0.2
 * \date 11/2015
 * \copyright David Krepsky
 */

#include "searchbar.h"
#include "ui_searchbar.h"
#include <QMessageBox>
#include "models/network.h"

/*******************************************************************************
* Constructor.
*******************************************************************************/
SearchBar::SearchBar(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SearchBar),
  bar_(NULL)
{
  ui->setupUi(this);

  // Create validator.
  // Since id won't be greater than 1000000, the max boundary is set to 1000000.
  ui->id->setValidator(new QIntValidator(0, 1000000, this));
}

/*******************************************************************************
* Destructor.
*******************************************************************************/
SearchBar::~SearchBar()
{
  delete ui;
}

/*******************************************************************************
* Set options.
*******************************************************************************/
void SearchBar::setOptions(Project *project)
{
  // Save project pointer and fill network combobox.
  project_ = project;

  foreach(Network *network, project->networks) {
    ui->networks->addItem(network->name);
  }
}

/*******************************************************************************
* Bar.
*******************************************************************************/
Bar *SearchBar::bar()
{
  return bar_;
}

/*******************************************************************************
* Accepted.
*******************************************************************************/
void SearchBar::on_search_clicked()
{
  // Check for empty data.
  if(ui->id->text().isEmpty()) {
    QMessageBox::information(this, "Invalid parameter", "Parameter ID is empty.",
                             QMessageBox::Ok);
    ui->id->setFocus();
    return;
  }

  // Check for empty data.
  if(ui->networks->currentIndex() == -1) {
    QMessageBox::information(this, "Invalid parameter",
                             "Parameter network is invalid.",
                             QMessageBox::Ok);
    ui->networks->setFocus();
    return;
  }

  // Search bar.
  Network *network = project_->networks.value(ui->networks->currentText());
  bar_ = network->getBarById(ui->id->text().toInt());

  if(bar_ == NULL) {
    QMessageBox::information(this, "Invalid parameter",
                             "Bar not found.",
                             QMessageBox::Ok);
    ui->id->setFocus();
    return;
  }

  accept();
}

/*******************************************************************************
* Rejected.
*******************************************************************************/
void SearchBar::on_cancel_clicked()
{
  reject();
}

/*!
 * \}
 */

#include "windowkflowsettings.h"
#include "ui_windowkflowsettings.h"

#include <QString>
#include <QValidator>
#include <QFileDialog>

WindowKflowSettings::WindowKflowSettings(QWidget *parent)
  : QDialog(parent), ui(new Ui::windowKflowSettings) {
  ui->setupUi(this);

  ui->maxIterations->setValidator(new QIntValidator(0, 1000000, this));
  ui->minError->setValidator(new QDoubleValidator(0, qInf(), 1000, this));
}

WindowKflowSettings::~WindowKflowSettings() {
  delete ui;
}

void WindowKflowSettings::setKflow(Kflow *kflow) {
  if (kflow == NULL) return;

  ui->path->setText(kflow->program());
  ui->maxIterations->setText(QString::number(kflow->maxIterations()));
  ui->minError->setText(QString::number(kflow->minError()));
  ui->saveIterations->setChecked(kflow->printIterations);
  ui->verbose->setChecked(kflow->verbose);

  kflow_ = kflow;
}

void WindowKflowSettings::on_btnCancel_clicked() {
  reject();
}

void WindowKflowSettings::on_btnPath_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
                       this, tr("Select kFlow"), qApp->applicationDirPath(),
                       tr("kFlow (KFlow KFlow.exe)"));

  if (!fileName.isEmpty()) ui->path->setText(fileName);
}

void WindowKflowSettings::on_btnOk_clicked() {
  if (!ui->path->text().isEmpty()) kflow_->setProgram(ui->path->text());

  if (!ui->maxIterations->text().isEmpty())
    kflow_->setMaxIterations(ui->maxIterations->text().toDouble());

  if (!ui->minError->text().isEmpty())
    kflow_->setMinError(ui->minError->text().toDouble());

  kflow_->printIterations = ui->saveIterations->isChecked();
  kflow_->verbose = ui->verbose->isChecked();
  accept();
}

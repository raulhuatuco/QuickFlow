#include "windownewproject.h"
#include "ui_windownewproject.h"

#include <QMessageBox>
#include <QFileDialog>

WindowNewProject::WindowNewProject(QWidget *parent)
    : QDialog(parent), ui(new Ui::WindowNewProject) {
  ui->setupUi(this);
}

WindowNewProject::~WindowNewProject() { delete ui; }

QString WindowNewProject::name() { return name_; }

QString WindowNewProject::path() { return path_.absolutePath(); }

void WindowNewProject::on_name_textChanged(const QString &arg1) {
  name_ = arg1;
  ui->outputFile->setText(path_.dirName() + QDir::separator() + arg1 +
                          ".qkflow");
}

void WindowNewProject::on_path_textChanged(const QString &arg1) {
  path_.setPath(arg1);
  ui->outputFile->setText(arg1 + QDir::separator() + name_ + ".qkflow");
}

void WindowNewProject::on_buttonBox_accepted() {
  if (ui->name->text().isEmpty()) {
    QMessageBox::information(this, "Invalid name", "Invalid project name",
                             QMessageBox::Ok);
    ui->name->setFocus();
    return;
  }
  if (ui->path->text().isEmpty()) {
    QMessageBox::information(this, "Invalid path", "Invalid project path",
                             QMessageBox::Ok);
    ui->path->setFocus();
    return;
  }

  if (!path_.exists()) {
    QMessageBox::information(this, "Invalid path", "Path does not exists.",
                             QMessageBox::Ok);
    ui->path->setFocus();
    return;
  }

  accept();
}

void WindowNewProject::on_toolButton_clicked() {
  QFileDialog directory(this);
  directory.setFileMode(QFileDialog::Directory);
  directory.setOption(QFileDialog::ShowDirsOnly);
  directory.setAcceptMode(QFileDialog::AcceptSave);
  directory.setWindowTitle("Project path");

  if (directory.exec()) ui->path->setText(directory.selectedFiles()[0]);
}

void WindowNewProject::on_buttonBox_rejected() { reject(); }

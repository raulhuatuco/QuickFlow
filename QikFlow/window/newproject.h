#ifndef WINDOWNEWPROJECT_H
#define WINDOWNEWPROJECT_H

#include <QDialog>
#include <QString>
#include <QDir>

namespace Ui {
class NewProject;
}

class NewProject : public QDialog {
  Q_OBJECT

 public:
  explicit NewProject(QWidget *parent = 0);
  ~NewProject();

  Ui::NewProject *ui;

 private slots:
  void on_name_textChanged(const QString &arg1);

  void on_path_textChanged(const QString &arg1);

  void on_buttonBox_accepted();

  void on_toolButton_clicked();

  void on_buttonBox_rejected();

 private:
};

#endif  // WINDOWNEWPROJECT_H

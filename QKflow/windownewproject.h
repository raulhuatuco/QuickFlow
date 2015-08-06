#ifndef WINDOWNEWPROJECT_H
#define WINDOWNEWPROJECT_H

#include <QDialog>
#include <QString>
#include <QDir>

namespace Ui {
class WindowNewProject;
}

class WindowNewProject : public QDialog {
  Q_OBJECT

 public:
  explicit WindowNewProject(QWidget *parent = 0);
  ~WindowNewProject();

  QString name();
  QString path();

 private slots:
  void on_name_textChanged(const QString &arg1);

  void on_path_textChanged(const QString &arg1);

  void on_buttonBox_accepted();

  void on_toolButton_clicked();

  void on_buttonBox_rejected();

 private:
  Ui::WindowNewProject *ui;
  QString name_;
  QDir path_;
};

#endif  // WINDOWNEWPROJECT_H

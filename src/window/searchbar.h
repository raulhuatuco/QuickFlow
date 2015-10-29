#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QDialog>
#include "project.h"

namespace Ui
{
class SearchBar;
}

class SearchBar : public QDialog
{
  Q_OBJECT

public:
  explicit SearchBar(QWidget *parent = 0);
  ~SearchBar();

  void setOptions(Project *project);

private:
  Ui::SearchBar *ui;
  Project *project_;

};

#endif  // SEARCHBAR_H

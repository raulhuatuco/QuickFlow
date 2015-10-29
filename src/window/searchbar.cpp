#include "searchbar.h"
#include "ui_searchbar.h"

#include "models/network.h"

SearchBar::SearchBar(QWidget *parent) : QDialog(parent), ui(new Ui::SearchBar)
{
  ui->setupUi(this);
}

SearchBar::~SearchBar()
{
  delete ui;
}

void SearchBar::setOptions(Project *project)
{
  project_ = project;

  foreach(Network *network, project->networks) {
    ui->networks->addItem(network->name);
  }
}

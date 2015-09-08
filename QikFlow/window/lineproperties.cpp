#include "lineproperties.h"
#include "ui_lineproperties.h"

LineProperties::LineProperties(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::LineProperties) {
  ui->setupUi(this);
}

LineProperties::~LineProperties() {
  delete ui;
}

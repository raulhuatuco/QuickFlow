#ifndef LINEPROPERTIES_H
#define LINEPROPERTIES_H

#include <QDialog>

namespace Ui {
class LineProperties;
}

class LineProperties : public QDialog {
  Q_OBJECT

 public:
  explicit LineProperties(QWidget *parent = 0);
  ~LineProperties();

 private:
  Ui::LineProperties *ui;
};

#endif // LINEPROPERTIES_H

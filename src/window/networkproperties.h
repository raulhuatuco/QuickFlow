#ifndef NEWNETWORK_H
#define NEWNETWORK_H

#include <QDialog>
#include "project.h"
#include "models/network.h"

namespace Ui
{
class NetworkProperties;
}

class NetworkProperties : public QDialog
{
  Q_OBJECT

public:
  explicit NetworkProperties(QWidget *parent = 0);
  ~NetworkProperties();

  void setOptions(Project *project, Network *network);
  
  Network *network();
  
private slots:
  void on_slackColor_clicked();

  void on_pqColor_clicked();

  void on_barBorderColor_clicked();

  void on_lineColor_clicked();

  void on_buttonBox_accepted();
  
  void on_buttonBox_rejected();
  
private:
  Ui::NetworkProperties *ui;
  Project *project_;
  Network *network_;
  bool isNew;
  QColor slack;
  QColor pq;
  QColor barBorder;
  QColor line;
};
#endif // NEWNETWORK_H

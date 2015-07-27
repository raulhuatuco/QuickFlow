#ifndef QKFLOW_H
#define QKFLOW_H

#include <QMainWindow>

namespace Ui {
class QKflow;
}

class QKflow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QKflow(QWidget *parent = 0);
    ~QKflow();

private:
    Ui::QKflow *ui;
};

#endif // QKFLOW_H

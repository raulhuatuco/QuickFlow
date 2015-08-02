#ifndef WINDOWADDSLACK_H
#define WINDOWADDSLACK_H

#include <QDialog>

#include "PnGraphics/pnslack.h"

namespace Ui {
class WindowAddSlack;
}

class WindowAddSlack : public QDialog
{
    Q_OBJECT

public:
    explicit WindowAddSlack(QWidget *parent = 0, PnSlack *slack = NULL);
    ~WindowAddSlack();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::WindowAddSlack *ui;
    PnSlack *slack_;
};

#endif // WINDOWADDSLACK_H

#include "quickflow.h"
#include <QApplication>

/*******************************************************************************
 * main.
 ******************************************************************************/
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QuickFlow w;
  w.show();

  return a.exec();
}

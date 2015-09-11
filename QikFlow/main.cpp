#include "qkflow.h"
#include <QApplication>

/*******************************************************************************
 * main.
 ******************************************************************************/
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QKflow w;
  w.show();

  return a.exec();
}

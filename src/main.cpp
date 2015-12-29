#include "quickflow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>

/*******************************************************************************
 * main.
 ******************************************************************************/
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QPixmap pixmap(":/splash.png");
  QSplashScreen *splash = new QSplashScreen(pixmap);
  splash->show();

  qApp->processEvents();
  QuickFlow w;
  
  QTimer::singleShot(2500, splash, SLOT(close()));
  QTimer::singleShot(2500, &w, SLOT(show()));
  
  return a.exec();
}

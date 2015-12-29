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
  
  // Load splash screen.
  QPixmap pixmap(":/splash.png");
  QSplashScreen *splash = new QSplashScreen(pixmap);
  splash->show();
  qApp->processEvents();
  
  // Create main window.
  QuickFlow w;
  
  // Set main window size accordingly to screen resolution.
  w.setGeometry(QApplication::desktop()->screenGeometry());
  
  // Close splash screen and open main windows after 2500 msec.
  QTimer::singleShot(2500, splash, SLOT(close()));
  QTimer::singleShot(2500, &w, SLOT(show()));
  
  // Process application messages.
  return a.exec();
}

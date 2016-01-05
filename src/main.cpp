/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \file main.cpp
 *
 * \brief Program entry point.
 *
 * This file contains the main function.
 *
 * \author David Krepsky
 * \version 1.0.0
 * \date 01/2016
 * \copyright David Krepsky
 */

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QDesktopWidget>
#include <QString>
#include "quickflow.h"

/*!
 * \brief Software version.
 */
QString version = "1.0.0";

/*!
 * \brief Software entry point.
 * \param[in] argc Number of arguments.
 * \param[in] argv Arguments.
 * \return Execution status.
 */
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  // Set application info.
  qApp->setOrganizationName("DKrepsky");
  qApp->setOrganizationDomain("dkrepsky.blogspot.com.br");
  qApp->setApplicationName("QuickFlow");
  qApp->setApplicationVersion(version);

  QSplashScreen *splash = new QSplashScreen();

  // Load splash screen if arg -nosplash is not present.
  if(!QApplication::arguments().contains("-nosplash")) {
    QPixmap pixmap(qApp->applicationDirPath() + QDir::separator() + "images" +
                   QDir::separator() + "splash.png");
    splash->setPixmap(pixmap);
    splash->show();
    splash->showMessage(QString("Version: ") + version,
                        Qt::AlignBottom | Qt::AlignRight);
    qApp->processEvents();
  }

  // Load and configure main window.
  QuickFlow w;
  QIcon appicon(qApp->applicationDirPath() + QDir::separator() + "images" +
                QDir::separator() + "appicon.png");
  qApp->setWindowIcon(appicon);
  w.setWindowIcon(appicon);

  // Show mainWindow.
  if(!QApplication::arguments().contains("-nosplash")) {
    // Close splash screen and open main windows after 2500 msec.
    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));
  } else {
    // Show it right way.
    w.show();
  }

  // Process application messages.
  return a.exec();
}

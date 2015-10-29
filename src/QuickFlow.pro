#-------------------------------------------------------------------------------
# The MIT License (MIT)
#
# Copyright (c) 2015 David Krepsky
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# QuickFlow Qt project file.
#
# This file provides all the necessary configuration to build QuickFlow for
# windows and linux.
#
# TODO: Add Apple IOS support.
#
# Author: David Krepsky
# Version: 0.1
# Date: 10/2015
# Copyright: David Krepsky
#-------------------------------------------------------------------------------

# Check Qt version (must be at least Qt 5).
lessThan(QT_MAJOR_VERSION, 5) {
  error("Qt version must be at least 5.");
}

# Add used Qt modules.
QT       += core gui widgets

# Target is an app named QuickFlow.
TARGET = QuickFlow
TEMPLATE = app

# Project source files.
SOURCES += main.cpp\
    quickflow.cpp \
    project.cpp \
    customtypes.cpp \
    window/lineproperties.cpp \
    window/newproject.cpp \
    window/barproperties.cpp \
    algorithms/shirmoharmnadi.cpp \
    models/bar.cpp \
    models/line.cpp \
    models/network.cpp \
    graphics/infobar.cpp \
    graphics/infoline.cpp \
    algorithms/import.cpp \
    algorithms/redraw.cpp \
    graphics/systemview.cpp \
    window/about.cpp \
    window/networkproperties.cpp \
    graphics/datatable.cpp \
    algorithms/radiallayer.cpp \
    algorithms/cespedes.cpp \
    window/searchbar.cpp
    
# Project header files.
HEADERS  += \
    quickflow.h\
    project.h \
    customtypes.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h \
    models/bar.h \
    models/line.h \
    models/network.h \
    graphics/infobar.h \
    graphics/infoline.h \
    algorithms/import.h \
    algorithms/redraw.h \
    graphics/systemview.h \
    algorithms/shirmoharmmadi.h \
    window/about.h \
    window/networkproperties.h \
    graphics/datatable.h \
    algorithms/radiallayer.h \
    algorithms/cespedes.h \
    math_constants.h \
    window/searchbar.h

# Forms.
FORMS    += \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui \
    quickflow.ui \
    window/about.ui \
    window/networkproperties.ui \
    window/searchbar.ui

# Resources.
RESOURCES += \
    img/img.qrc \

# Windows configuration.
win32 {
  # Set exe icon.
  RC_ICONS = AppIcon.ico

  # Add OGDF include directory.
  INCLUDEPATH += "..\third-party\ogdf\include"

  # Include OGDF and coin libraries.
  LIBS += -logdf -lcoin

  # Check build type.
  CONFIG(debug, debug|release) {
    # Add debug directive.
    DEFINES += OGDF_DEBUG

    # Check whether architecture is x86 or x64 to include libraries.
    contains(QT_ARCH, i386) {
      message("Building (Debug) for Windows x86.")
      LIBS += "-L..\third-party\ogdf\debug\lib\x86"
    } else {
      message("Building (Debug) for Windows x64.")
      LIBS += "-L..\third-party\ogdf\debug\lib\x64"
    }
  } else {
    # Release build.

    # Check whether architecture is x86 or x64 to include libraries.
    contains(QT_ARCH, i386) {
      message("Building (Release) for Windows x86.")
      LIBS += "-L..\third-party\ogdf\release\lib\x86"
    } else {
      message("Building (Release) for Windows x64.")
      LIBS += "-L..\third-party\ogdf\release\lib\x64"
    }
  }
}

# Linux configuration.
linux {
   CONFIG += c++11
   INCLUDEPATH += "..\third-party\ogdf\include"
   LIBS += -lOGDF -lCOIN -lpthread
}

# Artistic Style beautifier.
# Use to format source code.
DISTFILES+=.astylerc

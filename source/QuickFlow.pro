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
# Version: 1.0.0
# Date: 02/2016
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
    window/quickflow.cpp \
    window/lineproperties.cpp \
    window/barproperties.cpp \
    window/about.cpp \
    window/networkproperties.cpp \
    window/searchbar.cpp \
    window/projectproperties.cpp \
    models/bar.cpp \
    models/line.cpp \
    models/network.cpp \
    models/project.cpp \
    graphics/infobar.cpp \
    graphics/infoline.cpp \
    graphics/systemview.cpp \
    graphics/datatable.cpp \
    algorithms/radiallayer.cpp \
    algorithms/cespedes.cpp \
    algorithms/import.cpp \
    algorithms/redrawnetwork.cpp \
    algorithms/shirmoharmmadi.cpp \
    algorithms/barreduction.cpp
    utils/unit.cpp \

# Project header files.
HEADERS  += \
    window/quickflow.h \
    window/lineproperties.h \
    window/barproperties.h \
    window/about.h \
    window/networkproperties.h \
    window/searchbar.h \
    window/projectproperties.h \
    models/bar.h \
    models/line.h \
    models/network.h \
    models/project.h \
    graphics/infobar.h \
    graphics/infoline.h \
    graphics/systemview.h \
    graphics/datatable.h \
    algorithms/import.h \
    algorithms/shirmoharmmadi.h \
    algorithms/radiallayer.h \
    algorithms/cespedes.h \
    algorithms/redrawnetwork.h \
    algorithms/barreduction.h
    utils/math_constants.h \
    utils/unit.h \

# Forms.
FORMS    += \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/quickflow.ui \
    window/about.ui \
    window/networkproperties.ui \
    window/searchbar.ui \
    window/projectproperties.ui

# Resources.
RESOURCES += \
    theme/bluesky/bluesky.qrc

# Windows configuration.
win32 {
  # Set exe icon.
  RC_ICONS = images/QuickFlow.ico

  # Add OGDF include directory.
  INCLUDEPATH += "third-party\ogdf\include"

  # Include OGDF and coin libraries.
  LIBS += -logdf -lcoin

  # Check build type.
  CONFIG(debug, debug|release) {
    # Add debug directive.
    DEFINES += OGDF_DEBUG

    # Check whether architecture is x86 or x64 to include libraries.
    contains(QT_ARCH, i386) {
      message("Building (Debug) for Windows x86.")
      LIBS += "-Lthird-party\ogdf\debug\lib\x86"
    } else {
      message("Building (Debug) for Windows x64.")
      LIBS += "-Lthird-party\ogdf\debug\lib\x64"
    }
  } else {
    # Release build.

    # Check whether architecture is x86 or x64 to include libraries.
    contains(QT_ARCH, i386) {
      message("Building (Release) for Windows x86.")
      LIBS += "-Lthird-party\ogdf\release\lib\x86"
    } else {
      message("Building (Release) for Windows x64.")
      LIBS += "-Lthird-party\ogdf\release\lib\x64"
    }
  }
}

# Linux configuration.
linux {
   CONFIG += c++11
   INCLUDEPATH += "third-party/ogdf/include"

   LIBS += -lOGDF -lCOIN -lpthread
}

# Artistic Style beautifier.
# Use to format source code.
DISTFILES+=.astylerc

#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T16:06:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QKflow
TEMPLATE = app


SOURCES += main.cpp\
        qkflow.cpp \
    PnGraphics/pnbar.cpp \
    PnGraphics/pnview.cpp \
    PnGraphics/pnline.cpp \
    PnGraphics/pnnetwork.cpp \
    project.cpp \
    window/lineproperties.cpp \
    window/newproject.cpp \
    window/barproperties.cpp

HEADERS  += qkflow.h \
    PnGraphics/pnbar.h \
    PnGraphics/pnview.h \
    PnGraphics/pnline.h \
    PnGraphics/pnnetwork.h \
    project.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h

FORMS    += qkflow.ui \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui

RESOURCES += \
    img.qrc \

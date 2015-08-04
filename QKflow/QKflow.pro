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
    PnGraphics/pnslack.cpp \
    PnGraphics/pnpq.cpp \
    PnGraphics/pnpv.cpp \
    PnGraphics/pnline.cpp \
    PnGraphics/pncable.cpp \
    PnGraphics/pninfobox.cpp \
    PnGraphics/pnnetwork.cpp \
    windowaddslack.cpp \
    windowaddpq.cpp \
    windowaddpv.cpp \
    windowaddcable.cpp \
    projectsettings.cpp \
    kflow.cpp

HEADERS  += qkflow.h \
    PnGraphics/pnbar.h \
    PnGraphics/pnview.h \
    PnGraphics/pnslack.h \
    PnGraphics/pnpq.h \
    PnGraphics/pnpv.h \
    PnGraphics/pnline.h \
    PnGraphics/pncable.h \
    PnGraphics/pninfobox.h \
    PnGraphics/pnnetwork.h \
    windowaddslack.h \
    windowaddpq.h \
    windowaddpv.h \
    windowaddcable.h \
    projectsettings.h \
    kflow.h

FORMS    += qkflow.ui \
    windowaddslack.ui \
    windowaddpq.ui \
    windowaddpv.ui \
    windowaddcable.ui

RESOURCES += \
    img.qrc \
    PnGraphics/pnrc.qrc

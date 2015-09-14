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
    graphics/pnbar.cpp \
    graphics/pninfobar.cpp\
    graphics/pnview.cpp \
    graphics/pnline.cpp \
    graphics/pninfoline.cpp\
    graphics/pnnetwork.cpp \
    project.cpp \
    window/lineproperties.cpp \
    window/newproject.cpp \
    window/barproperties.cpp \
    unit.cpp \
    solver/shirmoharmnadi.cpp \
    graphics/pnlayer.cpp

HEADERS  += qkflow.h \
    graphics/pnbar.h \
    graphics/pninfobar.h\
    graphics/pnview.h \
    graphics/pnline.h \
    graphics/pninfoline.h\
    graphics/pnnetwork.h \
    project.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h \
    unit.h \
    solver/shirmoharmnadi.h \
    graphics/pnlayer.h

FORMS    += qkflow.ui \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui

RESOURCES += \
    img.qrc \

LIBS+= -llapack -lblas -larmadillo

DISTFILES+=.astylerc

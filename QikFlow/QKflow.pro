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
    project.cpp \
    window/lineproperties.cpp \
    window/newproject.cpp \
    window/barproperties.cpp \
    solver/shirmoharmnadi.cpp \
    pn/bar.cpp \
    pn/infobar.cpp \
    pn/infoline.cpp \
    pn/layer.cpp \
    pn/line.cpp \
    pn/network.cpp \
    pn/view.cpp \
    pn/types.cpp
    

HEADERS  += qkflow.h \
    project.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h \
    solver/shirmoharmnadi.h \
    pn/bar.h \
    pn/infobar.h \
    pn/infoline.h \
    pn/layer.h \
    pn/line.h \
    pn/network.h \
    pn/view.h \
    pn/types.h
    

FORMS    += qkflow.ui \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui

RESOURCES += \
    img.qrc \

LIBS+= -llapack -lblas -larmadillo

DISTFILES+=.astylerc
